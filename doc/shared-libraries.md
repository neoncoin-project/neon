Shared Libraries
================

## neonconsensus

The purpose of this library is to make the verification functionality that is critical to Neon's consensus available to other applications, e.g. to language bindings.

### API

The interface is defined in the C header `neonconsensus.h` located in `src/script/neonconsensus.h`.

#### Version

`neonconsensus_version` returns an `unsigned int` with the API version *(currently `1`)*.

#### Script Validation

`neonconsensus_verify_script` returns an `int` with the status of the verification. It will be `1` if the input script correctly spends the previous output `scriptPubKey`.

##### Parameters
- `const unsigned char *scriptPubKey` - The previous output script that encumbers spending.
- `unsigned int scriptPubKeyLen` - The number of bytes for the `scriptPubKey`.
- `const unsigned char *txTo` - The transaction with the input that is spending the previous output.
- `unsigned int txToLen` - The number of bytes for the `txTo`.
- `unsigned int nIn` - The index of the input in `txTo` that spends the `scriptPubKey`.
- `unsigned int flags` - The script validation flags *(see below)*.
- `neonconsensus_error* err` - Will have the error/success code for the operation *(see below)*.

##### Script Flags
- `neonconsensus_SCRIPT_FLAGS_VERIFY_NONE`
- `neonconsensus_SCRIPT_FLAGS_VERIFY_P2SH` - Evaluate P2SH ([BIP16](https://github.com/neon/bips/blob/master/bip-0016.mediawiki)) subscripts
- `neonconsensus_SCRIPT_FLAGS_VERIFY_DERSIG` - Enforce strict DER ([BIP66](https://github.com/neon/bips/blob/master/bip-0066.mediawiki)) compliance
- `neonconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY` - Enforce NULLDUMMY ([BIP147](https://github.com/neon/bips/blob/master/bip-0147.mediawiki))
- `neonconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY` - Enable CHECKLOCKTIMEVERIFY ([BIP65](https://github.com/neon/bips/blob/master/bip-0065.mediawiki))
- `neonconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY` - Enable CHECKSEQUENCEVERIFY ([BIP112](https://github.com/neon/bips/blob/master/bip-0112.mediawiki))
- `neonconsensus_SCRIPT_FLAGS_VERIFY_WITNESS` - Enable WITNESS ([BIP141](https://github.com/neon/bips/blob/master/bip-0141.mediawiki))

##### Errors
- `neonconsensus_ERR_OK` - No errors with input parameters *(see the return value of `neonconsensus_verify_script` for the verification status)*
- `neonconsensus_ERR_TX_INDEX` - An invalid index for `txTo`
- `neonconsensus_ERR_TX_SIZE_MISMATCH` - `txToLen` did not match with the size of `txTo`
- `neonconsensus_ERR_DESERIALIZE` - An error deserializing `txTo`
- `neonconsensus_ERR_AMOUNT_REQUIRED` - Input amount is required if WITNESS is used

### Example Implementations
- [NNeon](https://github.com/NicolasDorier/NNeon/blob/master/NNeon/Script.cs#L814) (.NET Bindings)
- [node-libneonconsensus](https://github.com/bitpay/node-libneonconsensus) (Node.js Bindings)
- [java-libneonconsensus](https://github.com/dexX7/java-libneonconsensus) (Java Bindings)
- [neonconsensus-php](https://github.com/Bit-Wasp/neonconsensus-php) (PHP Bindings)
