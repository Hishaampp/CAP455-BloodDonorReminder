Data folder

sample_donors_input.txt
  Adds six sample donors, shows the donor table and the reminder list, then exits.
  Run it from the project folder:

      ./donor_app < data/sample_donors_input.txt
      ./donor_app 05-11-2026 < data/sample_donors_input.txt   (fixed test date)

donors.dat
  The binary file where donors are saved (CA2). The program makes it the first
  time a donor is added and loads it every time it starts. Every donor takes the
  same number of bytes (one DonorRecord), so record n starts at byte
  n * sizeof(DonorRecord). Delete this file to start with an empty list.

  The donors.dat in the repo is a sample: the six donors from
  sample_donors_input.txt, 6 x 108 bytes = 648 bytes.
