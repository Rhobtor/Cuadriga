import pstats

# Load the statistics from the file
p = pstats.Stats('init_profile_results.prof')

# Sort and print the stats
p.strip_dirs().sort_stats('time').print_stats()

