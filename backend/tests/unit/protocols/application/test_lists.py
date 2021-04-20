"""Test the functionality of protocols.application.states classes."""

from ventserver.protocols.application import lists
from ventserver.protocols.protobuf import mcu_pb as pb


def test_send_new_elements() -> None:
    """Test adding new elements to a list for sending."""
    example_sequence = [
        lists.UpdateEvent(new_elements=[pb.LogEvent(id=i)])
        for i in range(20)
    ]

    synchronizer = lists.SendSynchronizer(
        segment_type=pb.NextLogEvents,
        max_len=10, max_segment_len=5
    )
    assert synchronizer.output() is None
    for update_event in example_sequence:
        synchronizer.input(update_event)

    assert synchronizer.output() is None

    # The first 10 events should've been discarded
    for next_expected in range(10):
        synchronizer.input(lists.UpdateEvent(next_expected=next_expected))
        output = synchronizer.output()
        assert isinstance(output, pb.NextLogEvents)
        assert output.next_expected == next_expected
        assert output.total == 10
        assert output.remaining == 10
        for (i, event) in enumerate(output.elements):
            assert event.id == 10 + i

    # Segments should be returned as requested
    for next_expected in range(10, 20):
        print(next_expected)
        synchronizer.input(lists.UpdateEvent(next_expected=next_expected))
        output = synchronizer.output()
        assert isinstance(output, pb.NextLogEvents)
        assert output.next_expected == next_expected
        assert output.total == 10
        assert output.remaining == 10 - (next_expected - 10)
        for (i, event) in enumerate(output.elements):
            assert event.id == next_expected + i
        if next_expected <= 15:
            assert len(output.elements) == 5
        else:
            assert len(output.elements) == 5 - (next_expected - 15)

    # New elements should be in the segment resulting from a repeated request
    assert synchronizer.output() is None
    synchronizer.input(lists.UpdateEvent(
        new_elements=[pb.LogEvent(id=20)], next_expected=19
    ))
    output = synchronizer.output()
    assert isinstance(output, pb.NextLogEvents)
    assert output.next_expected == 19
    assert output.total == 10
    assert output.remaining == 2
    for (i, event) in enumerate(output.elements):
        assert event.id == 19 + i
    assert len(output.elements) == 2

# TODO: add a test where we send all events, then reset expected event to 0.
# All events should be sent again.


def test_receive_new_elements() -> None:
    """Test adding new elements to a list from receiving."""
    example_sequence = [
        pb.NextLogEvents(elements=[pb.LogEvent(id=i) for i in range(0, 5)]),
        pb.NextLogEvents(elements=[pb.LogEvent(id=i) for i in range(5, 10)]),
        pb.NextLogEvents(elements=[pb.LogEvent(id=i) for i in range(7, 11)]),
        pb.NextLogEvents(elements=[pb.LogEvent(id=i) for i in range(0, 4)]),
    ]

    synchronizer: lists.ReceiveSynchronizer[pb.LogEvent] = \
        lists.ReceiveSynchronizer()
    assert synchronizer.output() is None
    for segment in example_sequence:
        synchronizer.input(segment)

    update_event = synchronizer.output()
    assert update_event is not None
    assert update_event.next_expected == 5
    assert len(update_event.new_elements) == 5
    for (i, element) in enumerate(update_event.new_elements):
        assert element.id == i

    update_event = synchronizer.output()
    assert update_event is not None
    assert update_event.next_expected == 10
    assert len(update_event.new_elements) == 5
    for (i, element) in enumerate(update_event.new_elements):
        assert element.id == 5 + i

    update_event = synchronizer.output()
    assert update_event is not None
    assert update_event.next_expected == 11
    assert len(update_event.new_elements) == 1
    assert update_event.new_elements[0].id == 10

    update_event = synchronizer.output()
    assert update_event is not None
    assert update_event.next_expected == 4
    assert len(update_event.new_elements) == 4
    for (i, element) in enumerate(update_event.new_elements):
        assert element.id == i
