# bmcweb Development Today: Setup & Workflow (07 July 2025)

This document summarizes the steps we performed today to get the OpenBMC **bmcweb** component cloned, built, configured, and deployed on an AST2600 QEMU target, including resolutions for all build-time and deployment issues.

---

## 1. Clone the bmcweb Repository

```bash
cd ~/workspace
git clone https://github.com/openbmc/bmcweb.git
cd bmcweb
```

---

## 2. Install Host Build Dependencies

```bash
sudo apt update
sudo apt install \\
  meson ninja-build pkg-config cmake \\
  libpam0g-dev libsystemd-dev libssl-dev \\
  libboost-all-dev nlohmann-json3-dev \\
  python3-yaml python3-mako python3-inflection \\
  libnghttp2-dev
```

- **libpam0g-dev** for PAM support  
- **python3-yaml, python3-mako, python3-inflection** for sdbusplus code generation

---

## 3. Choose Feature Flags (meson_options.txt)

We enabled the following features and set the C++ standard to 20:

- `cpp_std=c++20`  
- `redfish=enabled`  
- `rest=enabled`  
- `vm-websocket=enabled`  
- `host-serial-socket=enabled`  
- `static-hosting=enabled`  
- `kvm=disabled`  
- `basic-auth=enabled`  
- `session-auth=enabled`  
- `xtoken-auth=enabled`  
- `cookie-auth=enabled`  
- `tests=disabled`  

---

## 4. Configure & Build with Meson/Ninja

```bash
# Clean previous build
rm -rf builddir

# Configure Meson
meson setup builddir \\
  -Dcpp_std=c++20 \\
  -Dredfish=enabled \\
  -Drest=enabled \\
  -Dvm-websocket=enabled \\
  -Dhost-serial-socket=enabled \\
  -Dstatic-hosting=enabled \\
  -Dkvm=disabled \\
  -Dbasic-auth=enabled \\
  -Dsession-auth=enabled \\
  -Dxtoken-auth=enabled \\
  -Dcookie-auth=enabled \\
  -Dtests=disabled

# Build
cd builddir
ninja
```

- We forced C++20 so that bmcweb’s top-level requirement is satisfied.
- If encountering `std::move_only_function` errors from the bundled sdbusplus, we overrode the compiler flags with:
  ```bash
  export CXXFLAGS="${CXXFLAGS:-} -std=c++2b"
  ```
  to enable C++23 features for sub-projects.

---

## 5. Deploy to QEMU

1. **Free up QEMU overlay space** (to allow replacing `/usr/bin/bmcweb`):
   ```bash
   ssh -p2222 root@127.0.0.1
   journalctl --vacuum-size=1M
   rm -rf /tmp/* /var/tmp/* /var/log/journal/*
   exit
   ```
2. **Copy the binary**:
   ```bash
   scp -P 2222 builddir/bmcweb root@127.0.0.1:/tmp/
   ```
3. **Replace and restart**:
   ```bash
   ssh -p2222 root@127.0.0.1 << 'EOF'
     systemctl stop bmcweb
     mv /tmp/bmcweb /usr/bin/bmcweb
     chmod +x /usr/bin/bmcweb
     systemctl start bmcweb
   EOF
   ```
4. **Verify**:
   ```bash
   curl -k https://<QEMU_IP>/redfish/v1/
   ```
   You should see the standard Redfish service root JSON.

---

## 6. Next Steps

- **Add your first custom Redfish schema** under `static/redfish/v1/schema/`.  
- **Implement the handler** in `redfish-core/lib/…`.  
- **Rebuild & test** iteratively using the above workflow.  
- Integrate with **webui-vue** for frontend display.

---

**Congratulations!** You now have a repeatable build & deploy pipeline for `bmcweb` on AST2600 with QEMU. Proceed to schema and handler development next.



----------------------


# OpenBMC webserver

This component attempts to be a "do everything" embedded webserver for OpenBMC.

## Features

The webserver implements a few distinct interfaces:

- DBus event websocket. Allows registering on changes to specific dbus paths,
  properties, and will send an event from the websocket if those filters match.
- OpenBMC DBus REST api. Allows direct, low interference, high fidelity access
  to dbus and the objects it represents.
- Serial: A serial websocket for interacting with the host serial console
  through websockets.
- Redfish: A protocol compliant, [DBus to Redfish translator](docs/Redfish.md).
- KVM: A websocket based implementation of the RFB (VNC) frame buffer protocol
  intended to mate to webui-vue to provide a complete KVM implementation.

## Protocols

bmcweb at a protocol level supports http and https. TLS is supported through
OpenSSL.

## AuthX

### Authentication

Bmcweb supports multiple authentication protocols:

- Basic authentication per RFC7617
- Cookie based authentication for authenticating against webui-vue
- Mutual TLS authentication based on OpenSSL
- Session authentication through webui-vue
- XToken based authentication conformant to Redfish DSP0266

Each of these types of authentication is able to be enabled or disabled both via
runtime policy changes (through the relevant Redfish APIs) or via configure time
options. All authentication mechanisms supporting username/password are routed
to libpam, to allow for customization in authentication implementations.

### Authorization

All authorization in bmcweb is determined at routing time, and per route, and
conform to the Redfish PrivilegeRegistry.

\*Note: Non-Redfish functions are mapped to the closest equivalent Redfish
privilege level.

## Configuration

bmcweb is configured per the
[meson build files](https://mesonbuild.com/Build-options.html). Available
options are documented in `meson_options.txt`

## Compile bmcweb with default options

```ascii
meson setup builddir
ninja -C builddir
```

If any of the dependencies are not found on the host system during
configuration, meson will automatically download them via its wrap dependencies
mentioned in `bmcweb/subprojects`.

## Use of persistent data

bmcweb relies on some on-system data for storage of persistent data that is
internal to the process. Details on the exact data stored and when it is
read/written can seen from the `persistent_data` namespace.

## TLS certificate generation

When SSL support is enabled and a usable certificate is not found, bmcweb will
generate a self-signed a certificate before launching the server. Please see the
bmcweb source code for details on the parameters this certificate is built with.

## Redfish Aggregation

bmcweb is capable of aggregating resources from satellite BMCs. Refer to
[AGGREGATION.md](https://github.com/openbmc/bmcweb/blob/master/docs/AGGREGATION.md)
for more information on how to enable and use this feature.
