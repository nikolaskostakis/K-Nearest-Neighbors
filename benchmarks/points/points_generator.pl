# The script need to be given as argument the number of points
# If it not given, it wil quit
$numArgs = $#ARGV + 1;
if ($numArgs != 1)
{
	print "\nTo use thescript: points_generator.pl <noof_points>\r\n\n";
	exit;
}
else
{
	print "Points to generate: $ARGV[0]\r\n"
}

# assuming the script will be run from top folder
open($fh, '>',"./benchmarks/points/gen_points_$ARGV[o].txt" ) or die "Shit!";
for $i (1..$ARGV[0]) {
	# Some coordinates
    print $fh "P$i 5 5\n";
}
close $fh;
print "done\n";
