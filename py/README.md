# Horus Python SDK

## Building

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

## Development

[`mypy`](https://github.com/python/mypy) is used for type-checking:

```sh
python3 -m pip install 'mypy>=1.11' 'types-protobuf>=5.27' 'typing-extensions>=4.6'
python3 -m mypy .
```

[`black`](https://github.com/psf/black) is used for formatting:

```sh
python3 -m pip install 'black>=24.8'
python3 -m black .
```
