sysadmin
========

Like system administrators in big companies, `sysadmin` configures things

Development
===========

Generally speaking, build as follows:

```
mkdir build
cd build
cmake ..
make check
make
```

`make check` runs only sysadmin's tests. If you wish to run the decibel-cpp tests, run
`make decibel-check`.

If this is your first time, you might have some dependencies which need to be installed
first. The included `third-party-build.sh` will download, build, then install those
dependencies. You will definitely need gcc, g++, and make at the very least in order
to build those dependencies.

Dependencies
============

sysadmin's dependencies are codified in its CMakeLists.txt files, but the key ones are
as follows:
    - decibel-cpp, which is included with sysadmin and built when sysadmin is built. It
      is a C++ wrapper around libuv, and additional abstractions built with folly
    - [libuv](https://github.com/libuv/libuv) is a C event loop library
    - [boost](http://www.boost.org/) is boost
    - [folly](https://github.com/facebook/folly) is a C++ library, primarily used for
      its excellent futures code
    - [protobufs](https://developers.google.com/protocol-buffers/), specifically proto 2,
      provides the API to sysadmin
    - [yaml-cpp](https://github.com/jbeder/yaml-cpp) provides YAML config file support

About
=====

sysadmin's main goal is to be a system configuration manager. It accomplishes that
by providing a typed key value database, on which you the user can create "hooks".
These hooks allow you to do one or both of the following things, in response to
keys or groups of keys changing their values:
    1) Render jinja2 based templates and save them to the system
    2) Run arbitrary scripts

For example, say you have a set of keys whose values reflect dhcp settings:

```
network.dhcp.endip = "192.168.99.200"
network.dhcp.lease = 720
network.dhcp.router_ip = "192.168.99.1"
network.dhcp.router_netmask = "255.255.255.0"
network.dhcp.startip = "192.168.99.100"
network.dhcp.static_assignments = []
```

Two hooks exist to service these keys:
    1) A jinja2 template which renders a [dnsmasq](http://www.thekelleys.org.uk/dnsmasq/doc.html)
       configuration file which reflects the current values
    2) A simple redirection script which knows to restart the dnsmasq
       service via systemd.

sysadmin allows you to change the value of one or more of these keys, then `commit` them
all at once, at which point the 2 hooks above are run. From a user's perspective, they need
only know what values they want, and sysadmin takes care of the messy, system level details
via its hooks.
