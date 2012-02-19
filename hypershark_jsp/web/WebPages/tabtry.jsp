<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>
	<head>
		<style type="text/css" media="screen">
			
			.menu { padding: 0; clear: both; }
			.menu li { display: inline; }
			.menu li a { background: #ccf; padding: 10px; float:left; border-right: 1px solid #ccf; border-bottom: none; text-decoration: none; color: #000; font-weight: bold;}
			.menu li.active a { background: #eef; }
			.content { float: left; clear: both; border: 1px solid #ccf; border-top: none; border-left: none; background: #eef; padding: 10px 20px 20px; width: 400px; }
		</style>
	</head>
	<body>
		<ul id="menu" class="menu">
			<li class="active"><a href="#description">Limerick One</a></li>
			<li><a href="#usage">Limerick Two</a></li>
			<li><a href="#download">Limerick Three</a></li>
		</ul>
		<div id="description" class="content">
			<h2>Limerick One</h2>
			<p>
				The limerick packs laughs anatomical<br />
			    In space that is quite economical,<br />

			    But the good ones I've seen<br />
			    So seldom are clean,<br />
			    And the clean ones so seldom are comical.
			</p>
		</div>
		<div id="usage" class="content">
			<h2>Limerick Two</h2>
			<p>

				Let my viciousness be emptied,<br />
			    Desire and lust banished,<br />
			    Charity and patience,<br />
			    Humility and obedience,<br />
			    And all the virtues increased.
			</p>
		</div>

		<div id="download" class="content">
			<h2>Limerick Three</h2>
			Hickere, Dickere Dock,<br />
		    A Mouse ran up the Clock,<br />
		    The Clock Struck One,<br />
		    The Mouse fell down,<br />

		    And Hickere Dickere Dock.
		</div>
		
		
		
				
		<script src="tab/jquery.js" type="text/javascript" charset="utf-8"></script>
		<script src="tab/jquery.tabify.js" type="text/javascript" charset="utf-8"></script>
		<script type="text/javascript">
			// <![CDATA[
				
			$(document).ready(function () {
				$('#menu').tabify();
				$('#menu2').tabify();
			});
					
			// ]]>
		</script>

	</body>
</html>