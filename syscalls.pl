#!/usr/bin/perl
while ( defined($line = <STDIN>) ) {
    chomp $line;
    $line =~ s/\s*\#.*$//;	# Strip comments and trailing blanks

    if ( $line =~ /^\s*([^\(]+)\s+([^\s\(]+)\s*\(([^\(]*)\)\s*$/ ) {
	$type = $1;
	$name = $2;
	$argv = $3;

	@args = split(/\s*\,\s*/, $argv);

	open(OUT, "> syscalls/${name}.c")
	    or die "$0: Cannot open syscalls/${name}.c\n";
	print OUT "#include \"syscommon.h\"\n";
	print OUT "_syscall", scalar(@args), "(", $type, ',', $name;
	$i = 0;
	foreach $arg ( @args ) {
	    print OUT ",", $arg, ",a",$i++;
	}
	print OUT ");\n";
	close(OUT);
    }
}
