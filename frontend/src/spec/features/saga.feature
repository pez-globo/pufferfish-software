Feature: Redux Saga Feature

    To check redux saga

    Scenario: To check if redux saga workers is running as expected
        Given Saga worker is initialized
        When Saga workers are defined
        Then Saga should run workers for socket connection & clock

    Scenario: To check if redux saga worker for clock works
        Given Saga Clock worker is initialized
        When Saga Clock worker is defined
        Then Saga worker should dispatch clock action with 1000ms delay