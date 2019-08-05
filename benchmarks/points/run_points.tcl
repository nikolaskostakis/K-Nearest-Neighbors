# cat benchmarks/points/points2.txt
# read_points benchmarks/points/points2.txt
read_points benchmarks/points/gen_points_1000000.txt
# print_element_array
create_kdTree
# print_kdTree
# print_array
find_nearest_neighbours -k 1 -point 4.2 6
# find_nearest_neighbours -k 2 -point 3 3
# find_neighbours_within_radius -r 0.4 -point 3 3
# find_neighbours_within_radius -r 4 -point 3 3
# find_neighbours_within_radius -r 40 -point 3 3
# start_gui
# history
clear_elements
exit
# exit
# find_nearest_neighbours -k 1 -point 2.4 4.4
