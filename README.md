browser-git
===========

![](https://travis-ci.com/cppchriscpp/browser-git.svg?branch=master)

# Introduction

GIT for the browser using [libgit2](https://libgit2.org/) compiled to WebAssembly with [Emscripten](https://emscripten.org).

This is forked off of [wasm-git](https://github.com/petersalomonsen/wasm-git) by [petersalomonsen](https://github.com/petersalomonsen/wasm-git/commits?author=petersalomonsen).

That repo is a lot more clean and less hacky - I needed a few new features, and kinda just slapped them in as fast as I could. I also don't plan on keeping this up to date
with mainline right now.

I strongly recommend checking out his version. Credit goes to him for figuring out how to compile this thing with emscripten and making it all work. It's seriously impressive.

## Added features
- Creating new branches (create-branch) command
- Push uses your current branch instead of origin/master
- Checkout command actually updates `HEAD`

# [Original] Demo in the browser

A simple demo in the browser can be found at:

https://githttpserverdemo.petersalomonsen.usw1.kubesail.io/

**Please do not abuse, this is open for you to test and see the proof of concept**

The sources for the demo can be found in the [githttpserver](https://github.com/petersalomonsen/githttpserver) project, which is a simple git server deployable to [kubernetes](https://github.com/kubernetes/kubernetes). Showing basic operations like cloning, edit files, add and commit, push and pull.

_Note_: The url above is Peter Salomonsen's original version. If you want to try it with new features, run the copy in the repo.

# Example WebWorker with pre built binaries

For running in the browser you should have your git interaction code in a [webworker](https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Using_web_workers). This is because of the use of synchronous http requests and long running operations that would block if running on the main thread.

Here's an example of a simple webworker that uses pre-built binaries from https://unpkg.com/wasm-git@0.0.1/

```js
var Module = {
    locateFile: function(s) {
      return 'https://unpkg.com/wasm-git@0.0.2/' + s;
    }
};

importScripts('https://unpkg.com/wasm-git@0.0.2/lg2.js');

Module.onRuntimeInitialized = () => {
    const lg = Module;

    FS.mkdir('/working');
    FS.mount(MEMFS, { }, '/working');
    FS.chdir('/working');    

    FS.writeFile('/home/web_user/.gitconfig', '[user]\n' +
                'name = Test User\n' +
                'email = test@example.com');

    // clone a local git repository and make some commits

    lg.callMain(['clone',`http://localhost:5000/test.git`, 'testrepo']);

    FS.readdir('testrepo');
}
```

You'll start the worker from your html with the tag:

`<script>new Worker('yourworker.js')</script>;`

The example above expects to find a git repository at `http://localhost:5000/test.git`. If you want to clone from github you'd need a proxy running locally because of [CORS](https://developer.mozilla.org/en-US/docs/Web/HTTP/CORS) restrictions that would prevent you
accessing github directly. For testing you can use the proxy found in [examples/webserverwithgithubproxy.js](examples/webserverwithgithubproxy.js)

# Building

**IMPORTANT: This depends on using an emscripten version with fixes made as of 2020-03-29. Use newer versions of emscripten**

## Dependencies
- cmake
- emscripten 
- nodejs 12 or later (installed with emscripten)

See [.travis.yml](.travis.yml) for a full build and test pipeline including installing emscripten.

## Setup

Run [setup.sh](setup.sh) first to download libgit2 and apply patches.

Note that the initial build may be very slow finding things such as pthreads. This is normal, keep waiting.

Given you have installed and activated emscripten, you can use the command `npm run build` to configure and build, and you'll find the 
resulting `lg2.js` / `lg2.wasm` under the generated `emscriptenbuild/examples` folder. This command will automatically patch the files
in the `libgit2` folder, as well.

If you want a release build (minified javascript, smaller wasm file) you can run `npm run build-release`. This is what you want to use
in any real apps.

An example for the browser (using webworkers) can be found in [examples/example_webworker.js](examples/example_webworker.js). You can start a webserver for this by running the [examples/webserverwithgithubproxy.js](examples/webserverwithgithubproxy.js) script, which will launch a http server at http://localhost:5000 with a proxy to github. Proxy instead of direct calls is needed because of CORS restrictions in a browser environment.
