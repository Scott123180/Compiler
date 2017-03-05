//http://stackoverflow.com/questions/32615926/use-javascript-to-get-a-random-image-from-google-images


function makeid()
{
  var text = "";
  var possible = "abcdefghijklmnopqrstuvwxyz";

  for (var i=0; i < 3; i++)
  {
    text += possible.charAt(Math.floor(Math.random() * possible.length));
  }
  
  return text;
}

var keyword = makeid();

$(document).ready(function(){

$.getJSON("http://api.flickr.com/services/feeds/photos_public.gne?jsoncallback=?",
{
    tags: keyword,
    tagmode: "any",
    format: "json"
},
function(data) {
    var rnd = Math.floor(Math.random() * data.items.length);

    var image_src = data.items[rnd]['media']['m'].replace("_m", "_b");

    $('body').css('background-image', "url('" + image_src + "')");

});

});
