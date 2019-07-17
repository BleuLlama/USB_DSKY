
// 1. go to http://svtsim.com/moonjs/agc.html
// 2. inspect any element
// 3. move to the javascript console
// 4. paste this code in:

$('body').on( 'keypress', 
	function( e ) { 
		if( e.key == 'v' ) { $("button[name='17']").click(); }
		if( e.key == 'n' ) { $("button[name='31']").click(); }
		if( e.key == '+' ) { $("button[name='27']").click(); }
		if( e.key == '-' ) { $("button[name='16']").click(); }

		if( e.key == '0' ) { $("button[name='0']").click(); }
		if( e.key == '1' ) { $("button[name='1']").click(); }
		if( e.key == '2' ) { $("button[name='2']").click(); }
		if( e.key == '3' ) { $("button[name='3']").click(); }
		if( e.key == '4' ) { $("button[name='4']").click(); }
		if( e.key == '5' ) { $("button[name='5']").click(); }
		if( e.key == '6' ) { $("button[name='6']").click(); }
		if( e.key == '7' ) { $("button[name='7']").click(); }
		if( e.key == '8' ) { $("button[name='8']").click(); }
		if( e.key == '9' ) { $("button[name='9']").click(); }


		if( e.key == 'c' ) { $("button[name='30']").click(); }
		if( e.key == 'p' ) { $("button[id='pro']").click(); }
		if( e.key == 'k' ) { $("button[name='25']").click(); }

		if( e.key == 'e' ) { $("button[name='28']").click(); }
		if( e.key == 'r' ) { $("button[name='18']").click(); }
	}
);

// 5. you now have keyboard shortcuts!