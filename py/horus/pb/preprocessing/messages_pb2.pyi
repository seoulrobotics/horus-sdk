from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class OccupancyClassification(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    OCCUPANCYCLASSIFICATION_UNSPECIFIED: _ClassVar[OccupancyClassification]
    FREE: _ClassVar[OccupancyClassification]
    OCCLUDED: _ClassVar[OccupancyClassification]
    STATIONARY_OCCUPIED: _ClassVar[OccupancyClassification]
OCCUPANCYCLASSIFICATION_UNSPECIFIED: OccupancyClassification
FREE: OccupancyClassification
OCCLUDED: OccupancyClassification
STATIONARY_OCCUPIED: OccupancyClassification

class OccupancyGrid(_message.Message):
    __slots__ = ("rows", "cols", "cells")
    ROWS_FIELD_NUMBER: _ClassVar[int]
    COLS_FIELD_NUMBER: _ClassVar[int]
    CELLS_FIELD_NUMBER: _ClassVar[int]
    rows: int
    cols: int
    cells: _containers.RepeatedScalarFieldContainer[int]
    def __init__(self, rows: _Optional[int] = ..., cols: _Optional[int] = ..., cells: _Optional[_Iterable[int]] = ...) -> None: ...

class OccupancyGridEvent(_message.Message):
    __slots__ = ("grid", "x_min", "x_max", "y_min", "y_max", "resolution")
    GRID_FIELD_NUMBER: _ClassVar[int]
    X_MIN_FIELD_NUMBER: _ClassVar[int]
    X_MAX_FIELD_NUMBER: _ClassVar[int]
    Y_MIN_FIELD_NUMBER: _ClassVar[int]
    Y_MAX_FIELD_NUMBER: _ClassVar[int]
    RESOLUTION_FIELD_NUMBER: _ClassVar[int]
    grid: OccupancyGrid
    x_min: float
    x_max: float
    y_min: float
    y_max: float
    resolution: float
    def __init__(self, grid: _Optional[_Union[OccupancyGrid, _Mapping]] = ..., x_min: _Optional[float] = ..., x_max: _Optional[float] = ..., y_min: _Optional[float] = ..., y_max: _Optional[float] = ..., resolution: _Optional[float] = ...) -> None: ...
