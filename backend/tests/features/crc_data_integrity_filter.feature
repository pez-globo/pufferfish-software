Feature: CRC Data Integrity Filter 
    
    Scenario: Generate CRC for input message
        Given I have a CRCSender object
        And Receive a message
        
        When The message is not none
        And Input is valid

        Then I should get a message with CRC header


    Scenario: Parse encoded data and extract payload
        Given I have a CRCReceiver object
        And Receive a message

        When The message is not None
        And The message has valid size
        And The message is not corrupted

        Then I should get the correct payload 