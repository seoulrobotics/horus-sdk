import asyncio, typing

T = typing.TypeVar("T")


class CachedAwaitable(typing.Generic[T]):
    """
    Helper which allows an `await`able object to be `await`ed multiple times, always returning the
    latest value (while only running the computation once).
    """

    _awaitable: "asyncio.Future[typing.Tuple[T]]"

    def __init__(self, awaitable: typing.Awaitable[T]) -> None:
        self._awaitable = asyncio.gather(awaitable)

    async def get(self) -> T:
        return (await self._awaitable)[0]
