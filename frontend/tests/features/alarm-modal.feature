Feature: Alarm Modal Feature

    To check if alarm modal step increment/decrement works fine

    Scenario: To check if alarm values are stored in redux
        Given I set alarm limit values
        When I commit changes to redux action
        Then I should get action values verified with triggered one's