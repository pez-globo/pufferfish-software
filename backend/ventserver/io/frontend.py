"""Generic support for frontends."""

import attr


@attr.s(auto_attribs=True)
class FrontendProps:
    """Defines driver properties for serial devices."""

    process_name: str = '/usr/lib/chromium-browser/chromium-browser-v7'
