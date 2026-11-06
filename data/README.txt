Data folder

sample_donors_input.txt
  Sample data for CA1. It registers six sample donors, shows the donor
  table and the reminder list, then exits. Run it from the project folder:

      donor_app < data/sample_donors_input.txt
      donor_app 05-11-2026 < data/sample_donors_input.txt   (fixed test date)

  In CA1 all donor records are kept in memory (an array of 100 Donor
  objects), so records are lost when the program closes.

donors.dat (from CA2)
  From CA2 the program saves donor records to this binary file, so the
  data survives a restart.
