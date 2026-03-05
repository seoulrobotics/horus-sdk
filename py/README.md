# Horus Python SDK

## Building

> [!NOTE]
>
> These instructions assume you are working from the **published
> [`horus-sdk`](https://github.com/seoulrobotics/horus-sdk) repository**, where
> the generated protobuf files (`horus/pb/`) are already committed.
>
> If you are working inside the **`horus` repo**, the `horus/pb/` files are not
> committed and must be generated first — see
> [Generating protobuf files](#generating-protobuf-files) below.

The Python package can be built using the following command (assuming that the
working directory is the repository root):

```sh
python3 -m pip install --upgrade build
python3 -m build
```

The package can then be installed with:

```sh
python3 -m pip install dist/horus-*-py3-none-any.whl
```

And tried with:

```sh
python3 examples/print_logs.py
```

## Generating protobuf files

When working inside the `horus` monorepo, `horus/pb/` contains only a stub
`__init__.py`. All `*_pb2.py`, `*.pyi`, `*_client.py`, and `*_handler.py` files
are generated from the `.proto` sources in `common/horus_pb/` by the `horus`
build tool, and written to `build/RelWithDebInfo/sdk/py/horus/pb/`.

Run the following from the **repository root** to generate and copy them into
the source tree, then build and install the wheel:

```sh
tools/bin/horus build 'sdk/py/horus/**'
cp -r build/RelWithDebInfo/sdk/py/horus/. sdk/py/horus/
cd sdk/py && python3 -m pip install --upgrade build && python3 -m build
python3 -m pip install dist/horus-*-py3-none-any.whl --force-reinstall
```

## Development

Make sure you are in a virtual environment:

```sh
python3 -m venv .env
source .env/bin/activate
```

To install the `horus` package locally, without having to rebuild every time you
do a change, you can use:

```sh
python3 -m pip install -e .
```

Alternatively, Horus can be used without installation:

```sh
PYTHONPATH=. python3 examples/print_logs.py
```

[`mypy`](https://github.com/python/mypy) is used for type-checking:

```sh
python3 -m pip install 'mypy~=1.11' 'types-protobuf~=5.27' 'typing-extensions~=4.6'
python3 -m mypy .
```

[`black`](https://github.com/psf/black) is used for formatting:

```sh
python3 -m pip install 'black~=24.8'
python3 -m black .
```
