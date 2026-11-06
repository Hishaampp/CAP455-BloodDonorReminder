# Blood Donor Eligibility Reminder Console

CAP455 · Object-Oriented Programming using C++ · Minor Project · Session 2026–27
Student: Hisham P P · Registration no.: 12604325 · Section: D1P2635 · Faculty: Adarsh Singh

A menu-driven C++ console program that keeps a register of blood donors and shows
who is eligible to donate today and who becomes eligible in the next 7 days, using
the NBTC 2017 donor-selection rules (age 18–65, first-time donors up to 60,
weight at least 45 kg, 90-day gap for male and 120-day gap for female donors).

## Build

From this folder, with g++ (Linux, macOS, or MinGW on Windows):

```
g++ -std=c++17 -Wall src/main.cpp src/Donor.cpp src/Utils.cpp -o donor_app
```

Code::Blocks: create a new Console Application project, add all five files from
`src`, then Build and Run.

Works with any compiler supporting C++11 or later. Turbo C++ is not supported.

## Run

```
donor_app                 # uses today's date
donor_app 05-11-2026      # test mode: treats 05-11-2026 as today
```

Test mode exists so that test runs give the same results on any day.

Load the six sample donors from the data folder:

```
donor_app < data/sample_donors_input.txt
```

## Tests

Re-run a recorded console session:

```
donor_app 05-11-2026 < tests/test1_normal_input.txt
donor_app 05-11-2026 < tests/test2_invalid_input.txt
donor_app 05-11-2026 < tests/test3_boundary_input.txt
```

The `*_output.txt` files are transcripts of these sessions with the typed input shown.

Automated checks for validators, date maths and the eligibility rule:

```
g++ -std=c++17 tests/test_validators.cpp src/Donor.cpp src/Utils.cpp -Isrc -o run_tests
run_tests
```

Expected result: `Total: 27 passed, 0 failed`.

## Folder layout

| Folder  | Contents |
|---------|----------|
| `src`   | `main.cpp` (menu and operations), `Donor.h/.cpp` (Donor class), `Utils.h/.cpp` (dates, validators, safe input) |
| `docs`  | CA1 proposal (Word and PDF), UML class diagram, flowcharts (`.png` plus Graphviz `.dot` source) |
| `data`  | `sample_donors_input.txt` (six sample donors); the binary data file `donors.dat` will be stored here from CA2 |
| `tests` | Test input files, output transcripts, automated validator tests |

## Features by phase

### CA1 – Design & Create (done)
- `Donor` entity class with static members, inline functions and a friend function
- Array of 100 `Donor` objects; pointers for search and traversal; `this` pointer; `std::string`
- Create: add donor with validation on every field and no duplicate Donor IDs
- Read: show all donors, search by ID, eligibility reminders, find donors by blood group

### CA2 – Persist & Search (planned)
- Constructors (default, parameterised, copy) and destructor
- Fixed-size binary records in `data/donors.dat`; search with `seekg`; update with `seekp`

### CA3 – Reuse & Delete (planned)
- Base and derived donor classes (whole blood, apheresis) with virtual functions
- Operator overloading, type conversion, `new`/`delete`, safe update and delete

### CA4 – Integrate & Defend (planned)
- Full CRUD, custom exception class, templates, STL `vector` with `sort`, `find_if`, `count_if`
- Advanced feature: CSV export of the reminder list, backup and restore

## AI use

AI assistance (Claude, Anthropic) was used in CA1.
# CAP455-BloodDonorReminder
