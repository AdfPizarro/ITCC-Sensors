var staticUrl = 'https://ilxa-4a7f2.firebaseio.com/logs.json';
$.getJSON(staticUrl, function(data) {
  //console.log(data);
  var result = [];

  var keys = Object.keys(data);
     keys.forEach(function(key){
         result.push(data[key]);
     });

  //console.log(result)
  $("#time").text("Tiempo sin sujetar el volante: "+ result.length*2 + " segundos")


  for (var i = 0; i < result.length; i++) {
      $("#table").append("<tr><td>"+result[i].date +"</td>"+"<td>"+result[i].car+"</td>"+"<td>"+result[i].status+"</td>"+"</tr>");
      console.log(result[i])
  }
});
