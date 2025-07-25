/*
 * Tencent is pleased to support the open source community by making Puerts available.
 * Copyright (C) 2020 Tencent.  All rights reserved.
 * Puerts is licensed under the BSD 3-Clause License, except for the third-party components listed in the file 'LICENSE' which may be subject to their corresponding license terms. 
 * This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this source code package.
 */

var global = global || globalThis || (function () { return this; }());

function pathNormalize(path) {
    let reversePathFrags = path.split('/').reverse();
    let newPathFrags = [];
    while (reversePathFrags.length > 0) {
        let el = reversePathFrags.pop();
        if (el != "" && el != ".") {
            if (el == ".." && newPathFrags.length > 0 && newPathFrags[newPathFrags.length - 1] != "..") {
                newPathFrags.pop();
            } else {
                newPathFrags.push(el);
            }
        }
    }
    return (path.startsWith('/') ? '/' : '') + newPathFrags.join("/");
}

function searchModuleInDirWithExt(dir, requiredModule) {
    var searchPath = pathNormalize((dir ? dir + '/' : '') + requiredModule);
    if (puer.fileExists(searchPath)) {
        return searchPath;
    }

    searchPath = pathNormalize(dir + '/node_modules/' + requiredModule);
    if (puer.fileExists(searchPath)) {
        return searchPath;
    }
}

function getFileExtension(filepath) {
    let last = filepath.split('/').pop();
    let frags = last.split('.');
    if (frags.length > 1) {
        return frags.pop();
    }
}


function searchModuleInDir(dir, requiredModule) {
    if (getFileExtension(requiredModule)) {
        return searchModuleInDirWithExt(dir, requiredModule)
            || searchModuleInDirWithExt(dir, requiredModule + "/index.js")
            || searchModuleInDirWithExt(dir, requiredModule + "/package.json");
    } else {
        return searchModuleInDirWithExt(dir, requiredModule + ".js")
            || searchModuleInDirWithExt(dir, requiredModule + ".cjs")
            || searchModuleInDirWithExt(dir, requiredModule + "/index.js")
            || searchModuleInDirWithExt(dir, requiredModule + "/package.json");
    }
}

function searchModule(dir, requiredModule) {
    var result = searchModuleInDir(dir, requiredModule);
    if (result) return result;
    if (dir != "" && !requiredModule.endsWith(".js")) {
        let pathFrags = dir.split('/');
        pathFrags.pop();
        pathFrags.unshift('');
        while (pathFrags.length > 0) {
            if (pathFrags[pathFrags.length - 1] != "node_modules") {
                result = searchModuleInDir(pathFrags.join("/"), requiredModule);
                if (result) return result;
            }
            pathFrags.pop();
        }
    }
}

puer.searchModule = searchModule;