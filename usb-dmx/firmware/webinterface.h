const char *home = R"(
  <html>
	  <head>
		  <title>TRANCE</title>
	  </head>

	  <body>
		  <div id='content'>
			  <h1>TRANCE</h1>
			  <form action=''>
				  <fieldset>
					  <legend>Wifi</legend>
  					  <label for='ssid'>SSID:</label>
  					  <input type='text' id='ssid' name='ssid'><br><br>
 			 		  <label for='psk'>PASSWORD:</label>
  					  <input type='text' id='psk' name='psk'><br><br>
  					  <input type='submit' value='connect'>
  				  </fieldset>
			  </form>

			  <form action=''>
				  <fieldset>
					  <legend>E1.31</legend>
 			 		  <label for='psk'>UNIVERSE:</label>
  					  <input type='number' id='universe' name='universe'><br><br>
  					  <input class='content' type='submit' value='connect'>
  				  </fieldset>
			  </form>
		  </div>
	  </body>

	  <style>
		  .content {
			  margin: 20px;
		  }

	  </style>

  </html>)";