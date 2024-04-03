#include "Common.h"

namespace PUERTS_NAMESPACE
{
    static const char* ExecuteModuleJSCode = "                                                      \n"
"(function() {                                                                                      \n"
"    var global = this;                                                                             \n"
"    /* eslint-disable max-depth, max-statements, complexity, max-lines-per-function */             \n"
"    const SLASH = 47                                                                               \n"
"    const DOT = 46                                                                                 \n"
"                                                                                                   \n"
"    const assertPath = (path) => {                                                                 \n"
"        const t = typeof path                                                                      \n"
"        if (t !== 'string') {                                                                      \n"
"            throw new TypeError(`Expected a string, got a ${t}`)                                   \n"
"        }                                                                                          \n"
"    }                                                                                              \n"
"                                                                                                   \n"
"    // this function is directly from node source                                                  \n"
"    const posixNormalize = (path, allowAboveRoot) => {                                             \n"
"        let res = ''                                                                               \n"
"        let lastSegmentLength = 0                                                                  \n"
"        let lastSlash = -1                                                                         \n"
"        let dots = 0                                                                               \n"
"        let code                                                                                   \n"
"                                                                                                   \n"
"        for (let i = 0; i <= path.length; ++i) {                                                   \n"
"            if (i < path.length) {                                                                 \n"
"                code = path.charCodeAt(i)                                                          \n"
"            } else if (code === SLASH) {                                                           \n"
"                break                                                                              \n"
"            } else {                                                                               \n"
"                code = SLASH                                                                       \n"
"            }                                                                                      \n"
"            if (code === SLASH) {                                                                  \n"
"                if (lastSlash === i - 1 || dots === 1) {                                           \n"
"                    // NOOP                                                                        \n"
"                } else if (lastSlash !== i - 1 && dots === 2) {                                    \n"
"                    if (                                                                           \n"
"                        res.length < 2 ||                                                          \n"
"                        lastSegmentLength !== 2 ||                                                 \n"
"                        res.charCodeAt(res.length - 1) !== DOT ||                                  \n"
"                        res.charCodeAt(res.length - 2) !== DOT                                     \n"
"                    ) {                                                                            \n"
"                        if (res.length > 2) {                                                      \n"
"                            const lastSlashIndex = res.lastIndexOf('/')                            \n"
"                            if (lastSlashIndex !== res.length - 1) {                               \n"
"                                if (lastSlashIndex === -1) {                                       \n"
"                                    res = ''                                                       \n"
"                                    lastSegmentLength = 0                                          \n"
"                                } else {                                                           \n"
"                                    res = res.slice(0, lastSlashIndex)                             \n"
"                                    lastSegmentLength = res.length - 1 - res.lastIndexOf('/')      \n"
"                                }                                                                  \n"
"                                lastSlash = i                                                      \n"
"                                dots = 0                                                           \n"
"                                continue                                                           \n"
"                            }                                                                      \n"
"                        } else if (res.length === 2 || res.length === 1) {                         \n"
"                            res = ''                                                               \n"
"                            lastSegmentLength = 0                                                  \n"
"                            lastSlash = i                                                          \n"
"                            dots = 0                                                               \n"
"                            continue                                                               \n"
"                        }                                                                          \n"
"                    }                                                                              \n"
"                    if (allowAboveRoot) {                                                          \n"
"                        if (res.length > 0) {                                                      \n"
"                            res += '/..'                                                           \n"
"                        } else {                                                                   \n"
"                            res = '..'                                                             \n"
"                        }                                                                          \n"
"                        lastSegmentLength = 2                                                      \n"
"                    }                                                                              \n"
"                } else {                                                                           \n"
"                    if (res.length > 0) {                                                          \n"
"                        res += '/' + path.slice(lastSlash + 1, i)                                  \n"
"                    } else {                                                                       \n"
"                        res = path.slice(lastSlash + 1, i)                                         \n"
"                    }                                                                              \n"
"                    lastSegmentLength = i - lastSlash - 1                                          \n"
"                }                                                                                  \n"
"                lastSlash = i                                                                      \n"
"                dots = 0                                                                           \n"
"            } else if (code === DOT && dots !== -1) {                                              \n"
"                ++dots                                                                             \n"
"            } else {                                                                               \n"
"                dots = -1                                                                          \n"
"            }                                                                                      \n"
"        }                                                                                          \n"
"                                                                                                   \n"
"        return res                                                                                 \n"
"    }                                                                                              \n"
"                                                                                                   \n"
"    const decode = (s) => {                                                                        \n"
"        try {                                                                                      \n"
"            return decodeURIComponent(s)                                                           \n"
"        } catch {                                                                                  \n"
"            return s                                                                               \n"
"        }                                                                                          \n"
"    }                                                                                              \n"
"                                                                                                   \n"
"    const normalize = (p) => {                                                                     \n"
"        assertPath(p)                                                                              \n"
"                                                                                                   \n"
"        let path = p                                                                               \n"
"        if (path.length === 0) {                                                                   \n"
"            return '.'                                                                             \n"
"        }                                                                                          \n"
"                                                                                                   \n"
"        const isAbsolute = path.charCodeAt(0) === SLASH                                            \n"
"        const trailingSeparator = path.charCodeAt(path.length - 1) === SLASH                       \n"
"                                                                                                   \n"
"        path = decode(path)                                                                        \n"
"        path = posixNormalize(path, !isAbsolute)                                                   \n"
"                                                                                                   \n"
"        if (path.length === 0 && !isAbsolute) {                                                    \n"
"            path = '.'                                                                             \n"
"        }                                                                                          \n"
"        if (path.length > 0 && trailingSeparator) {                                                \n"
"            path += '/'                                                                            \n"
"        }                                                                                          \n"
"        if (isAbsolute) {                                                                          \n"
"            return '/' + path                                                                      \n"
"        }                                                                                          \n"
"                                                                                                   \n"
"        return path                                                                                \n"
"    }                                                                                              \n"
"                                                                                                   \n"
"    global.__puer_path__ = {                                                                       \n"
"        normalize: normalize,                                                                      \n"
"        isAbsolute(filepath) {                                                                     \n"
"            return !(                                                                              \n"
"                !/^[\\\\\\/]{2,}[^\\\\\\/]+[\\\\\\/]+[^\\\\\\/]+/.test(filepath) &&                \n"
"                !/^([a-z]:)?[\\\\\\/]/i.test(filepath)                                             \n"
"            )                                                                                      \n"
"        },                                                                                         \n"
"        isRelative(filepath) {                                                                     \n"
"            if (filepath[0] == '.') {                                                              \n"
"                if (filepath.length == 1 || filepath[1] == '/') return true;                       \n"
"                if (filepath[1] == '.') {                                                          \n"
"                    if (filepath.length == 2 || filepath[2] == '/') return true;                   \n"
"                }                                                                                  \n"
"            }                                                                                      \n"
"            return false;                                                                          \n"
"        },                                                                                         \n"
"        dirname: function dirname(path) {                                                          \n"
"            if (path.length === 0) return '.';                                                     \n"
"            var code = path.charCodeAt(0);                                                         \n"
"            var hasRoot = code === 47 /*/*/;                                                       \n"
"            var end = -1;                                                                          \n"
"            var matchedSlash = true;                                                               \n"
"            for (var i = path.length - 1; i >= 1; --i) {                                           \n"
"                code = path.charCodeAt(i);                                                         \n"
"                if (code === 47 /*/*/) {                                                           \n"
"                    if (!matchedSlash) {                                                           \n"
"                        end = i;                                                                   \n"
"                        break;                                                                     \n"
"                    }                                                                              \n"
"                    } else {                                                                       \n"
"                    // We saw the first non-path separator                                         \n"
"                    matchedSlash = false;                                                          \n"
"                }                                                                                  \n"
"            }                                                                                      \n"
"                                                                                                   \n"
"            if (end === -1) return hasRoot ? '/' : '.';                                            \n"
"            if (hasRoot && end === 1) return '//';                                                 \n"
"            return path.slice(0, end);                                                             \n"
"        }                                                                                          \n"
"    }                                                                                              \n"
"                                                                                                   \n"
"    var loader = typeof jsEnv != 'undefined' ? jsEnv.GetLoader() : __tgjsGetLoader();              \n"
"    global.__puer_resolve_module_url__ = function(specifier, referer) {                            \n"
"        const originSp = specifier;                                                                \n"
"        if (!loader.Resolve) {                                                                     \n"
"            let s = !__puer_path__.isRelative(specifier) ? specifier :                             \n"
"                __puer_path__.normalize(__puer_path__.dirname(referer) + '/' + specifier)          \n"
"            if (loader.FileExists(s)) {                                                            \n"
"                return s                                                                           \n"
"            } else {                                                                               \n"
"                throw new Error(`[Puer002]import ${originSp} failed: module not found`);           \n"
"            }                                                                                      \n"
"                                                                                                   \n"
"        } else {                                                                                   \n"
"            let p = loader.Resolve(specifier, referer)                                             \n"
"            if (!p) {                                                                              \n"
"                throw new Error(`[Puer002]import ${originSp} failed: module not found`);           \n"
"            }                                                                                      \n"
"            return p;                                                                              \n"
"        }                                                                                          \n"
"    }                                                                                              \n"
"    global.__puer_resolve_module_content__ = function(specifier, debugpathRef = []) {              \n"
"        const originSp = specifier;                                                                \n"
"                                                                                                   \n"
"        let isESM = true;                                                                          \n"
"        if (loader.IsESM) isESM = specifier.startsWith('puerts/') || loader.IsESM(specifier)       \n"
"        if (!isESM && puer.require) return `export default puer.require('${specifier}')`           \n"
"        const content = loader.ReadFile(specifier, debugpathRef);                                  \n"
"        if (!content) {                                                                            \n"
"            throw new Error(`[Puer003]import ${originSp} failed: module not found`);               \n"
"        }                                                                                          \n"
"        return content                                                                             \n"
"    }                                                                                              \n"
"})();";                                                                                            
}
