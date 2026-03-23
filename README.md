# logos-zone-sequencer-module

A Logos Core IComponent module that enables zone inscription on the Logos blockchain via [zone-sdk](https://github.com/logos-blockchain/logos-blockchain/tree/main/zone-sdk).

## Architecture

```
Logos App / yolo-ng
    ↓ invokeRemoteMethod (QtRO)
logos-zone-sequencer-module (C++ Qt plugin)
    ↓ FFI
zone-sequencer-rs (Rust, embeds tokio + zone-sdk)
    ↓ HTTP
Logos blockchain node
    ↓
Devnet / Mainnet
```

## Q_INVOKABLE Interface

| Method | Description |
|--------|-------------|
| `set_node_url(url)` | HTTP endpoint of the blockchain node (default: `http://localhost:8080`) |
| `set_signing_key(hex)` | Ed25519 signing key as 64-char hex (32 bytes). Channel ID is derived automatically from its public key. |
| `set_checkpoint_path(path)` | File path to load/save sequencer checkpoint for chain continuity. Required for txs to land on-chain. |
| `publish(data)` | Inscribes `data` as a zone block. Returns local tx hash on success, error string on failure. |

## Usage via logoscore

```bash
logoscore -m ~/.local/share/Logos/LogosAppNix/modules \
  -l liblogos_zone_sequencer_module \
  -c "liblogos_zone_sequencer_module.set_node_url(http://192.168.0.209:8080)" \
  -c "liblogos_zone_sequencer_module.set_signing_key(<64-char-hex-key>)" \
  -c "liblogos_zone_sequencer_module.set_checkpoint_path(/tmp/sequencer.checkpoint)" \
  -c "liblogos_zone_sequencer_module.publish(hello world)"
```

> **Note:** The checkpoint file is required for transactions to be accepted by validators. Without a valid checkpoint, the sequencer has no chain context and txs will be rejected. On first use, bootstrap by running `logos-blockchain-node inscribe` once and copying its checkpoint file.

## Building

Requires:
- Qt 6.9.2 (nixpkgs `e9f00bd8`)
- [zone-sequencer-rs](https://github.com/jimmy-claw/zone-sequencer-rs) built first

```bash
# 1. Build the Rust FFI lib
git clone https://github.com/jimmy-claw/zone-sequencer-rs ../zone-sequencer-rs
cd ../zone-sequencer-rs && cargo build --release

# 2. Build the Qt plugin
cd logos-zone-sequencer-module
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=<qt6-path>
make -j$(nproc)
```

## Installing

```bash
MODULES_DIR=~/.local/share/Logos/LogosAppNix/modules
cp build/liblogos_zone_sequencer_module.so $MODULES_DIR/
cp ../zone-sequencer-rs/target/release/libzone_sequencer_rs.so $MODULES_DIR/
```

## Repos

- Qt module: https://github.com/jimmy-claw/logos-zone-sequencer-module
- Rust FFI: https://github.com/jimmy-claw/zone-sequencer-rs
