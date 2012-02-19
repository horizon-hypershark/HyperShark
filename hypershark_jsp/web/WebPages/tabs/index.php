<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Progtuts.info | JS Examples</title>

<link rel="stylesheet" href="style.css" type="text/css" media="screen" title="Flora (Default)">
<script type="text/javascript" src="jquery.js"></script>
<script type="text/javascript" src="jquery-ui.js"></script>
<script type="text/javascript" src="jquery-tabs.js"></script>


<script>
	// The following function is called once the rest of the document is fully loaded
	$(document).ready(function()
	{
		$("#tabs > ul").tabs(); // Make the tabs.
	});
</script>
  
</head>
<body>
	<div id="tabs" class="tabs">
		<ul>
			<li><a href="#example"><span>One</span></a></li>
			<li><a href="another_file.html"><span>Two</span></a></li>
		</ul>
		
		<div id="example">
			<p>
			Some text in this file.
			</p>
		</div>
	</div>
</body>
</html>
