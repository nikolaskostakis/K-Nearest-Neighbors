read_points benchmarks/points2.txt
create_kdTree
find_nearest_neighbours -k 1 -point 3 3
find_nearest_neighbours -k 5 -point 3 3
find_neighbours_within_radius -r 4 -point 3 3
find_neighbours_within_radius -r 40 -point 3 3
# exit
# find_nearest_neighbours -k 1 -point 2.4 4.4
exit