Package: gumbo:x64-linux@0.12.3

**Host Environment**

- Host: x64-linux
- Compiler: GNU 11.4.0
-    vcpkg-tool version: 2025-04-07-b8b513ba8778c918cff49c3e837aae5999d5d2aa
    vcpkg-scripts version: df8bfe5195 2025-04-10 (5 hours ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
Downloading https://codeberg.org/gumbo-parser/gumbo-parser/archive/0.12.3.tar.gz -> gumbo-0.12.3.tar.gz
error: curl: not found
note: If you are using a proxy, please ensure your proxy settings are correct.
Possible causes are:
1. You are actually using an HTTP proxy, but setting HTTPS_PROXY variable to `https//address:port`.
This is not correct, because `https://` prefix claims the proxy is an HTTPS proxy, while your proxy (v2ray, shadowsocksr, etc...) is an HTTP proxy.
Try setting `http://address:port` to both HTTP_PROXY and HTTPS_PROXY instead.
2. If you are using Windows, vcpkg will automatically use your Windows IE Proxy Settings set by your proxy software. See: https://github.com/microsoft/vcpkg-tool/pull/77
The value set by your proxy might be wrong, or have same `https://` prefix issue.
3. Your proxy's remote server is our of service.
If you believe this is not a temporary download server failure and vcpkg needs to be changed to download this file from a different location, please submit an issue to https://github.com/Microsoft/vcpkg/issues
CMake Error at scripts/cmake/vcpkg_download_distfile.cmake:136 (message):
  Download failed, halting portfile.
Call Stack (most recent call first):
  /home/jevgome/.cache/vcpkg/registries/git-trees/34c97da27c275c1657e50d6598c72b8aca5cb8cd/portfile.cmake:3 (vcpkg_download_distfile)
  scripts/ports.cmake:206 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "dependencies": [
    "curl",
    "libxml2",
    "gumbo"
  ]
}

```
</details>
