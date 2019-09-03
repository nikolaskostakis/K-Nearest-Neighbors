# cat benchmarks/points/points2.txt
# read_points benchmarks/points/points2.txt
# read_points benchmarks/points/gen_points_1000000.txt
read_points benchmarks/points/gen_points_1000.txt
# print_element_array
create_kdTree
# print_kdTree
print_sorting_array
find_nearest_neighbors -noofneighbors 1 -point 4.2 6
find_nearest_neighbors -noofneighbors 2 -point 3 3
# find_neighbors_within_radius -radius 0.4 -point 3 3
find_neighbors_within_radius -radius 4 -point 3 3
# find_neighbors_within_radius -radius 40 -point 3 3
# start_gui
# history
clear_elements
# exit
quit
# exit
