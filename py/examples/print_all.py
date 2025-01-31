import asyncio, horus, signal


async def print_all() -> None:
    sdk = horus.Sdk()
    end = asyncio.Future()

    asyncio.get_running_loop().add_signal_handler(
        signal.SIGINT, lambda: end.set_result(None)
    )

    async with sdk.subscribe_to_logs(on_log_message=print), sdk.subscribe_to_profiling(
        on_profiling_info=print
    ):
        await end


asyncio.run(print_all())
