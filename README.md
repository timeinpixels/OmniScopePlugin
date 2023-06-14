# OmniScopePlugin

Generic OmniScope plugin structure acting as an input plugin (available in the Connect To window).
The plugin has the UI inteface capabilities - both the input settings as well as permanent UI acces:

1. Creating new windows
2. Displaying popups and message boxes

The plugin should be compiled to dll (windows) or dylib (on macOS) and placed in a predefined directory that OmniScope will scan on startup.
