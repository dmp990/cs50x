-- Keep a log of any SQL queries you execute as you solve the mystery.


-- Get the sense of tables and schema
.tables

-- Get me the schema of crime_scene_reports
.schema crime_scene_reports

-- Ran this query in order to get the list of crimes committed on July 28, 2021
SELECT * FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28;

-- Further narrow down the list to only include crimes committed on Humphrey Street on that fateful day
SELECT * FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street";

-- Only get the the details of the crime that I have been nominated to solve. Don't worry about other petty crimes like homicide
SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street" AND description LIKE '%duck%';
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their
-- interview transcripts mentions the bakery.

-- cuz I have bad memory from years of sleepless nights spent chasing duck-thieves
.tables

-- Get the transcript of interviews to collect the different pieces of puzzle
SELECT transcript FROM interviews WHERE month = 7 AND day = 28 AND year = 2021 AND transcript LIKE '%bakery%';

-- 1st breadcrumb: Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- Get the license plates of suspects
SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 30 AND activity = "exit";

-- 2nd breadcrumb: Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street
-- and saw the thief there withdrawing some money.
-- Get the account number of people who withdrew from that atm on that day on that street
SELECT account_number FROM atm_transactions
WHERE atm_location = "Leggett Street" AND year = 2021 AND month = 7 AND day = 28 AND transaction_type = "withdraw";

-- 3rd breadcrumb: they called someone who talked to them for less than a minute. In the call,
-- I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.
-- Get the list of caller, receiver pairs when ^^ conditions meet and check to see who purchased flights
SELECT caller, receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;

-- Narrow down the list:
-- Get the id (id of the person) of suspects who fit the description from first two breadcrumbs
SELECT id
FROM people
JOIN bank_accounts
ON people.id = bank_accounts.person_id
WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions
WHERE atm_location = "Leggett Street" AND year = 2021 AND month = 7 AND day = 28 AND transaction_type = "withdraw")
AND license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 30 AND activity = "exit");

-- Get the list of people whose phone numbers is in the list of callers from breadcrumb 3 -> suspected thieves
SELECT id FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND id IN
(SELECT id FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions
WHERE atm_location = "Leggett Street" AND year = 2021 AND month = 7 AND day = 28 AND transaction_type = "withdraw")
AND license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 30 AND activity = "exit"));


-- Get me list of suspect accomplices
SELECT people.id FROM people WHERE phone_number IN (
SELECT receiver FROM phone_calls WHERE caller IN (SELECT phone_number FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND id IN
(SELECT id FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions
WHERE atm_location = "Leggett Street" AND year = 2021 AND month = 7 AND day = 28 AND transaction_type = "withdraw")
AND license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 30 AND activity = "exit"))
AND year = 2021  AND month = 7 AND day = 28 AND duration < 60));

-- Get passport numbers of suspected thief and check to see if took the flight on 29/7/2021 from Fiftyville early morning
SELECT passport_number FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND id IN
(SELECT id FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions
WHERE atm_location = "Leggett Street" AND year = 2021 AND month = 7 AND day = 28 AND transaction_type = "withdraw")
AND license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 30 AND activity = "exit"));

-- get the flight ids of flights of suspects
SELECT * FROM passengers WHERE flight_id IN
(SELECT id FROM flights
WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = (SELECT id FROM airports WHERE full_name LIKE '%Fiftyville%'))
AND passport_number IN (SELECT passport_number FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND id IN
(SELECT id FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE bank_accounts.account_number IN
(SELECT account_number FROM atm_transactions
WHERE atm_location = "Leggett Street" AND year = 2021 AND month = 7 AND day = 28 AND transaction_type = "withdraw")
AND license_plate IN
(SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 30 AND activity = "exit")));
-- flight_id -> 18, 36. Must be 36 because 18 departed later in the day. Passport number: 5773159633

-- Get the name of thief
SELECT name FROM people
WHERE passport_number = 5773159633;
-- Bruce is the name of thief

-- Get the name of departure airport
SELECT city FROM airports WHERE id = (SELECT destination_airport_id FROM flights WHERE id = 36);
-- went to new york city

-- who did the thief call on the fatefull day
SELECT name FROM people WHERE phone_number = (
SELECT receiver FROM phone_calls
WHERE caller = (SELECT phone_number FROM people
WHERE passport_number = 5773159633)
AND year = 2021 AND month = 7 AND day = 28 and duration < 60);
-- Robin is the accomplice

-- WOOF, that was by far the hardest pset of CS50