import pytest
import struct
import hftc

@pytest.mark.benchmark(group="add")
def test_add(benchmark):
    benchmark(hftc.add, 123, 456)

@pytest.mark.benchmark(group="ring")
def test_ring_push_pop(benchmark):
    # micro-benchmark one push+pop
    hftc.ring_init()
    def push_pop():
        hftc.ring_push(42)
        hftc.ring_pop()
    benchmark(push_pop)

@pytest.mark.benchmark(group="parse")
def test_parse_message(benchmark):
    msg = struct.pack('<QdI', 1, 123.45, 10)
    benchmark(hftc.parse_message, msg)

@pytest.mark.benchmark(group="encode")
def test_encode_order(benchmark):
    benchmark(hftc.encode_order, 1, 42, 'B', 123.45, 10)
