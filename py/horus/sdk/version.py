import dataclasses, typing
import horus.pb.status_service.service_pb2 as ss_pb

if typing.TYPE_CHECKING:
    from typing_extensions import override
else:
    override = lambda f: f


@dataclasses.dataclass(frozen=True)
class Version:
    """Horus version."""

    major: int
    """Major version."""
    minor: int
    """Minor version."""
    patch: int
    """Patch version."""
    pre: str
    """pre-release version qualifier such as "beta". May be empty."""

    @staticmethod
    def _from_pb(pb: ss_pb.GetVersionResponse) -> "Version":
        return Version(
            major=pb.version.major,
            minor=pb.version.minor,
            patch=pb.version.patch,
            pre=pb.version.pre,
        )

    @override
    def __str__(self) -> str:
        preSep = "-" if len(self.pre) > 0 else ""
        return f"{self.major}.{self.minor}.{self.patch}{preSep}{self.pre}"
