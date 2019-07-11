cat benchmarks/points/points2.txt
read_points benchmarks/points/points2.txt
print_hash
create_kdTree
print_array
find_nearest_neighbours -k 1 -point 3 3
find_nearest_neighbours -k 5 -point 3 3
find_neighbours_within_radius -r 4 -point 3 3
find_neighbours_within_radius -r 40 -point 3 3
# start_gui
clear_points
exit
# exit
# find_nearest_neighbours -k 1 -point 2.4 4.4
