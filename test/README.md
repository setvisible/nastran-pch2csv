# Tests

 - `/filemanager`    
        Contains the tests for the `FileManager` class.

 - `/scanner`    
        **End-to-end test**.
        Contains the automatic unit tests (requires the Qt framework, i.e. QtTest) for the classes `Reader`, `Writer` and `PunchFile`. The class `Scanner` is a simple container that runs and verifies the workflow.

 - `/csvcomparer`    
        The `CSVComparer` class is a helper class that compares two [CSV](https://en.wikipedia.org/wiki/Comma-separated_values "Comma-Separated Values (CSV)") files. It compares the data independently of its storage format.

