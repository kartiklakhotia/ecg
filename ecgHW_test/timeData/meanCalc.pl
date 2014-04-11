#!/usr/local/bin/perl



my $filename = "TraversalOrderReversed_fourUnrolled";

open (fh, '<', $filename);

@test = <fh>;
close fh;

my $count = 0;
my $sum = 0;
my $numSamples = 0;
my $min = 10;
my $max = 0;

foreach (@test){
	$count = $count + 1;
	if ($count%3 == 0){
		my @chars = split(" ", $_);
		print "$_       $count\n";
		my $val = $chars[3];	
	#	print "${val}  ";
		if ($val < 0.55){
			$sum = $sum + $val;
			$numSamples = $numSamples + 1;
			if ($val > $max) {
				$max = $val;
			}
			if ($val < $min) {
				$min = $val;
			}
		}
	}
}
my $mean = $sum/$numSamples;
print "mean is $mean\n";
print "minimum is $min\n";
print "maximum is $max\n";
