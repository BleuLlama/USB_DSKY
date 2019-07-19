
// Add keyboard shortcuts for the emulated AGC/DSKY at http://svtsim.com/moonjs/agc.html


//// Create a new bookmark in your browser, and set the URL to this:
// (this creates a 'bookmarklet')

javascript:(function(){ 
	/* remap start */
	$('body').on( 'keypress', 
		function( e ) { 
			if( e.key == 'v' ) { $("button[name='17']").click(); }
			if( e.key == 'n' ) { $("button[name='31']").click(); }

			if( e.key == '+' ) { $("button[name='26']").click(); }
			if( e.key == '-' ) { $("button[name='27']").click(); }

			if( e.key == '0' ) { $("button[name='16']").click(); }
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
	/* remap end */	
})();


// then load the page: http://svtsim.com/moonjs/agc.html
//	and select that javascript bookmarklet

// or you can paste the code between the /* remap start */ and /* remap end */ to the
// javascript console on your browser when you've loaded that page.
