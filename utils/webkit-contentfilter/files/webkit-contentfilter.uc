#!/usr/bin/ucode
'use strict';

import { cursor } from 'uci';
import { glob, open, readfile, writefile, rename, unlink, mkdir, lsdir,
	 readlink, symlink, access, error as fs_error } from 'fs';
import * as uloop from 'uloop';
import * as uclient from 'uclient';
import { convert } from 'webkit-contentfilter.convert';

const CONFIG = 'webkit-contentfilter';
const PUBLISH_DIR = '/var/run/webkit-contentfilter';
const DEFAULT_STORAGE = '/var/lib/webkit-contentfilter';
const RECHECK_INTERVAL_MS = 24 * 60 * 60 * 1000;
/* maxRuleCount in WebKit's ContentExtensionParser.cpp */
const WEBKIT_MAX_RULES = 150000;

let ca_files = glob('/etc/ssl/certs/*.crt');
let recheck_timer;

function dir_create(path) {
	let current = '';

	for (let part in split(path, '/')) {
		if (part == '')
			continue;
		current += '/' + part;
		mkdir(current, 0755);
	}
}

function config_load() {
	let uci = cursor();
	let config = {
		storage: uci.get(CONFIG, 'global', 'storage') ?? DEFAULT_STORAGE,
		sources: []
	};

	uci.foreach(CONFIG, 'source', function(s) {
		if (s.enabled == '0' || !s.url)
			return;
		push(config.sources, { name: s['.name'], url: s.url });
	});

	return config;
}

function meta_load(path) {
	try {
		return json(readfile(path) ?? '{}');
	}
	catch (e) {
		return {};
	}
}

function rules_write(path, rules) {
	let tmp = path + '.new';
	let f = open(tmp, 'w');

	if (!f)
		return false;

	f.write('[');
	for (let i = 0; i < length(rules); i++)
		f.write((i ? ',\n' : '\n') + sprintf('%J', rules[i]));
	f.write('\n]\n');
	f.close();

	return rename(tmp, path);
}

function list_publish(config, name) {
	let target = `${config.storage}/${name}.json`;
	let link = `${PUBLISH_DIR}/${name}.json`;

	if (readlink(link) == target)
		return;

	unlink(link);
	symlink(target, link);
}

function list_store(config, source, text, headers) {
	let path = `${config.storage}/${source.name}.json`;
	let result = convert(text);
	let count = length(result.rules);

	warn(`${source.name}: ${count} rules, ${sprintf('%J', result.stats)}\n`);

	if (!count || count > WEBKIT_MAX_RULES) {
		warn(`${source.name}: ${count} rules cannot be compiled by WebKit\n`);
		return;
	}

	if (!rules_write(path, result.rules)) {
		warn(`${source.name}: writing ${path} failed: ${fs_error()}\n`);
		return;
	}

	writefile(`${config.storage}/${source.name}.meta`, sprintf('%J\n', {
		url: source.url,
		etag: headers.etag,
		last_modified: headers['last-modified']
	}));

	list_publish(config, source.name);
}

function request_headers(config, source) {
	let meta = meta_load(`${config.storage}/${source.name}.meta`);
	let headers = {};

	if (meta.url != source.url ||
	    !access(`${config.storage}/${source.name}.json`, 'r'))
		return headers;

	if (meta.etag)
		headers['If-None-Match'] = meta.etag;
	if (meta.last_modified)
		headers['If-Modified-Since'] = meta.last_modified;

	return headers;
}

function source_fetch(config, source, done) {
	let chunks = [];
	let finished;
	let status;
	let cl;

	let finish = function(err) {
		if (finished)
			return;
		finished = true;

		if (err)
			warn(`${source.name}: ${source.url}: ${err}\n`);
		else if (status == 304)
			list_publish(config, source.name);
		else if (status == 200)
			list_store(config, source, join('', chunks), cl.get_headers());
		else
			warn(`${source.name}: ${source.url}: HTTP status ${status}\n`);

		cl = null;
		chunks = null;
		done();
	};

	cl = uclient.new(source.url, null, {
		header_done: function() {
			let st = cl.status();

			if (st.redirect) {
				cl.redirect();
				return;
			}
			status = st.status;
		},
		data_read: function() {
			let data;

			while (length(data = cl.read()) > 0)
				if (status == 200)
					push(chunks, data);
		},
		data_eof: function() {
			finish(null);
		},
		error: function(cb, code) {
			finish(`uclient error ${code}`);
		}
	});

	if (!cl)
		return finish('invalid URL');

	if (!cl.ssl_init({ ca_files, verify: true }))
		return finish('TLS setup failed');

	if (!cl.connect())
		return finish('connection failed');

	if (!cl.request('GET', { headers: request_headers(config, source) }))
		finish('request failed');
}

function stale_unpublish(config) {
	let names = map(config.sources, s => `${s.name}.json`);

	for (let file in lsdir(PUBLISH_DIR) ?? [])
		if (index(names, file) < 0)
			unlink(`${PUBLISH_DIR}/${file}`);
}

function sources_next(config, pending) {
	let source = shift(pending);

	if (!source)
		return recheck_timer.set(RECHECK_INTERVAL_MS);

	source_fetch(config, source, () => sources_next(config, pending));
}

function sources_update() {
	let config = config_load();

	dir_create(config.storage);
	dir_create(PUBLISH_DIR);
	stale_unpublish(config);
	sources_next(config, [ ...config.sources ]);
}

uloop.init();
recheck_timer = uloop.timer(-1, sources_update);
sources_update();
uloop.run();
