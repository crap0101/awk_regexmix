
@load "regexmix"

@include "awkpot"
# https://github.com/crap0101/awkpot
@include "testing"
# https://github.com/crap0101/awk_testing

BEGIN {

    testing::start_test_report()

    # TEST make
    split("1", a, ":")
    a[0] = @/^.*?x+9$/
    # a[1] is strnum 1
    a[2] = "foo"
    a[3]
    a[4] = ""
    a[5] = 19
    a[6] = invented
    for (i in a) {
        r = regexmix::make(a[i])
	testing::assert_equal(typeof(r), "regexp", 1, sprintf("> make regexp from <%s> (%s)", a[i], typeof(a[i])));
    }

    testing::assert_equal(typeof(regexmix::make(1, 2)), "unassigned", 1, "> wrong call, too many args: returns unassigned")
    cmd = sprintf("%s -l regexmix 'BEGIN {regexmix::make()}'", ARGV[0])
    testing::assert_false(awkpot::exec_command(cmd), 1, "> wrong call, no args: fatal")

    # report
    testing::end_test_report()
    testing::report()

}
