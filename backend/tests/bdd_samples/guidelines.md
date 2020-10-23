This document describes guidelines to write unit tests for the ventserver modules. We are loosely adhering to BDD (Behaviour Driven Development) style development process, keeping the unit tests writing and description guidelines while leaving the tests first approach. This will help us ensure that we write tests for all the crucial components of our system while not hampering the speed and flow of the development. For further reading [here](https://dannorth.net/introducing-bdd/).

There two reasons for using BDD:
1. The BDD requries us to write the tests first and then the actual components that satisfies those tests. The BDD tests are basically requirement specification described in plain language. Developing the components while considering the tests will ensure that we're always in line with the system requirements.
2. The generated tests documentation could be easily integrated with the MatrixReq documentation.

## How to describe the test:
We'll follow BDD based tags for writing unit test descriptions.
    
    Story: (Module docstring) Describe where and how, functionally, the module is being used in an abstract and simple language.
    
    Scenario: (Method/Test docstring) Describe the fuctionality for which the unit test is being written. Multi-line description is accepted. Keep the language as siple and abstract as possible.
    
    Given: (Method/Test docstring) Describe in single line what are prerequisites for the unit test like intilization of sender/receiver objects, connection endpoints, e.t.c. Assertion on this should always pass.
    
    When: (Method/Test docstring) Describe conditions that that should be met like when event_time == current_time, e.t.c. There can be similar Given steps for multiple tests.
    
    Then: (Method/Test docstring) Describe the expected outcome from the functionality.

    And: (Method/Test docstring) Additional steps to be defined. Since, Given, When, and, Then tags are used to describe a single constraint/step, And is used along with them to define multiple contraints for the tag.

Basic syntax example:

test_some_module.py
```
"""
Story The module gives example of how BDD style can be integrated with docstrings.
"""

@hp.given(...)
@pt.parametrize(...)
def test_this_functionality(args):
    """Scenario: Sample unit test to show how to integrate tags with docstrings.
    """
    # Given: The understanding of the functionality.
    Object initialization code B.
    # And: The knowledge of BDD. 
    Object initialization code B.

    # When: The unit tests are written and conditions are met.
    Some condition code.
    
      # Then: The test result should show pass.
      Code to verify the result.
```

Example test could be found in `backend/tests/bdd_samples`

Since, we have been using test suites that maybe incompatible with BDD style test suites, we'll continue using old test suites but we'll design the tests and write the docstrings according to BDD directions. This will help us reduce any dependency based complications that can arise in the future and let us stick to the test libraries that we've been using. 

## Prioritization and Risk Assesment

Try to follow the MatrixReq documentation for thoroughly understanding the requirements and risks associated with the component. Design and prioritize the unit tests considering the risk associated with it. The risks of the component can be computed from the severity and probability of the failure. For more info go to [this blog](https://www.automatetheplanet.com/unit-testing-guidelines/).

Considering risks associated with them, we have listed some tests cases with higher priority for the following type of components:

Protocols & Filters:

1. Funtionality of methods in Sender and Receiver classes
2. Exceptions raised by Sender and Receiver classes
3. One-way trip from input through output for both Sender and Receiver classes
4. Round trip from input of Receiver to output of Sender from all the way through output of Sender and input of Receiver.
5. Any additional utility classes or funtions. 

Endpoints
    
1. Includes everything mentioned in Protocols and Filter in addition to communication with the I/O device using simulated values. 
