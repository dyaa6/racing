String update = R"(
    <!DOCTYPE html>
<html lang='ar'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Settings</title>
    </head>
    <body>
<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
  <input type='file' name='update'>
  <input type='submit' value='Update'>
</form>
<div id='prg'>progress: 0%</div>
<script>
  document.getElementById('upload_form').addEventListener('submit', function(e) {
    e.preventDefault();
    var form = this;
    var data = new FormData(form);
    var xhr = new XMLHttpRequest();
    xhr.open('POST', '/update', true);
    xhr.upload.onprogress = function(e) {
      if (e.lengthComputable) {
        var percent = (e.loaded / e.total) * 100;
        document.getElementById('prg').innerHTML = 'نسبة التقدم: ' + percent.toFixed(0) + '%';
      }
    };
    xhr.onload = function() {
      if (xhr.status === 200) {
        console.log('Upload successful!');
      } else {
        console.error('Upload failed!');
      }
    };
    xhr.send(data);
  });
</script>
</body>
)";