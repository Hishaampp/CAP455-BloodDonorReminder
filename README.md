# Blood Donor Eligibility Reminder Console

CAP455 · Object-Oriented Programming using C++ · Minor Project · Session 2026–27
Student: Hisham P P · Reg. no. 12604325 · Section D1P2635 · Faculty: Adarsh Singh

A menu-driven C++ console program that keeps a list of blood donors and tells who
can donate today and who can donate in the next 7 days. It uses the NBTC 2017 rules:
age 18–65 (first-time donors up to 60), weight at least 45 kg, and a gap of
90 days for male and 120 days for female donors.

## Build

Run this from the project folder (g++ on Linux, macOS or MinGW on Windows):

```
g++ -std=c++17 -Wall src/main.cpp src/Donor.cpp src/Utils.cpp src/FileHandler.cpp -o donor_app
```

Code::Blocks: make a new Console Application, add all the files from `src`, then Build and Run.

## Run

Always run it from the project folder, because donors are saved in `data/donors.dat`.

```
./donor_app                 # uses today's date
./donor_app 05-11-2026      # test mode: uses 05-11-2026 as today
```

Load the six sample donors:

```
./donor_app < data/sample_donors_input.txt
```

## Menu

```
1. Add new donor
2. Show all donors
3. Search donor by ID          (reads from the file using seekg)
4. Eligibility reminders (today + next 7 days)
5. Find donors by blood group
6. Update donor                (phone, city or new donation date, saved with seekp)
0. Exit
```

## Tests

Delete `data/donors.dat` before test 1, 2 and 3 so they start with an empty file.

```
./donor_app 05-11-2026 < tests/test1_normal_input.txt
./donor_app 05-11-2026 < tests/test4_update_input.txt          # run after test 1
./donor_app 05-11-2026 < tests/test5_after_restart_input.txt   # run after test 4
./donor_app 05-11-2026 < tests/test2_invalid_input.txt
./donor_app 05-11-2026 < tests/test3_boundary_input.txt
```

Test 4 and test 5 show that the data is still there after the program is closed
and opened again, and that the update was saved in the file.
The `*_output.txt` files are the saved outputs of these runs.

Automatic checks (validation, dates, eligibility, constructors, file functions):

```
g++ -std=c++17 -Wall tests/test_validators.cpp src/Donor.cpp src/Utils.cpp src/FileHandler.cpp -Isrc -o run_tests
./run_tests
```

Expected result: `Total: 44 passed, 0 failed`.

## Folders

| Folder  | What is inside |
|---------|----------------|
| `src`   | `main.cpp` (menu), `Donor.h/.cpp` (Donor class), `Utils.h/.cpp` (dates, validation, input), `FileHandler.h/.cpp` (binary file) |
| `docs`  | Proposal, UML class diagram, flowcharts |
| `data`  | `sample_donors_input.txt`; `donors.dat` is made by the program |
| `tests` | Test inputs, saved outputs, automatic checks |

## Features by phase

### CA1 – Design & Create (done)
- `Donor` class with static members, inline functions and a friend function (`daysLeft`)
- Array of 100 `Donor` objects, pointers for searching, `this` pointer, `std::string`
- Add donor with validation on every field and no duplicate Donor IDs
- Show all, search by ID, reminders, find by blood group

### CA2 – Persist & Search (done)
- Default, parameterised and copy constructors, and a destructor
- `DonorRecord` struct with fixed-size char arrays, written to `data/donors.dat` in binary
- All donors are loaded again when the program starts, so data survives a restart
- Search by ID jumps through the file with `seekg`
- Update phone, city or last donation date, written back in place with `seekp` (asks Y/N first)

### CA3 – Reuse & Delete (planned)
- Base and derived donor classes (whole blood, apheresis) with virtual functions
- Operator overloading, type conversion, `new`/`delete`, safe update and delete

### CA4 – Integrate & Defend (planned)
- Full CRUD, custom exception class, templates, STL `vector` with `sort`, `find_if`, `count_if`
- CSV export of the reminder list, backup and restore

## AI use

I used AI (Claude, Anthropic) to help me understand and clean up the code in CA1 and CA2.
I have gone through every part and can explain it.
