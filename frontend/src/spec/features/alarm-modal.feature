Feature: Alarm Modal Feature

    To check if alarm modal step increment/decrement works fine

    Scenario: To check if Alarm increment is working
        Given I mount Alarm modal component
        When I click on increment button
        Then I should get the result as incremented value

    Scenario: To check if Alarm decrement is working
        Given I mount Alarm modal component
        When I click on decrement button
        Then I should get the result as decremented value

    Scenario: To check if confirm button is working
        Given I set alarm limit values
        When I click on confirm button
        Then I should get confirm action called with values I have set

    Scenario: To check if alarm values are stored in redux
        Given I set alarm limit values
        When I commit changes to redux action
        Then I should get action values verified with triggered one's