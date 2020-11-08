class Node {
  constructor(name, lat, lng, color) {
    this.name = name;
    this.lat = lat;
    this.lng = lng;
    this.color=color;
  }
}

class Line
{
  constructor(a, b, color) {
    this.a = a;
    this.b = b;
    this.color=color;
  }
}

  var styleJson = 'https://api.maptiler.com/maps/e37057d7-2e5c-4978-838e-ce1022159874/style.json?key=oB33yoiaSZxO5iGNOflP';
  var map = new ol.Map({
          target: 'map',
          view: new ol.View({
            center: ol.proj.fromLonLat([7.6280, 46.7580]),
            zoom: 8
          })
        });
  olms.apply(map, styleJson);

function plotPoint(a)
{
  var myStyle = new ol.style.Style({
      image: new ol.style.Circle({
        radius: 7,
        fill: new ol.style.Fill({color: 'black'}),
        stroke: new ol.style.Stroke({
          color: [255,0,0], width: 2
        })
      })
    })

  var color = percentageToHsl(a.color, 120, 0);
   var layer = new ol.layer.Vector({
     source: new ol.source.Vector({
         features: [
             new ol.Feature({
                 geometry: new ol.geom.Point(ol.proj.fromLonLat([a.lat, a.lng])),
               color: color
             })
         ]
     })
 });
layer.setZIndex(1);
 return layer;
}

function percentageToHsl(percentage, hue0, hue1) {
    var hue = (percentage * (hue1 - hue0)) + hue0;
    return 'hsl(' + hue + ', 100%, 50%)';
}


function percentageToHsl(percentage, hue0, hue1) {
    var hue = (percentage * (hue1 - hue0)) + hue0;
    return 'hsl(' + hue + ', 100%, 50%)';
}



function plotLine(a,b, color)
{

  var color = percentageToHsl(color, 65, 0);
 var styles = [
        // linestring
        new ol.style.Style({
          stroke: new ol.style.Stroke({
            color: color,
            width: 2.5
          })
        })
      ];
  var lonlat1 = ol.proj.fromLonLat([a.lat, a.lng]);
  var location1 = ol.proj.fromLonLat([b.lat, b.lng]);
  var line = new ol.Feature({
                 geometry: new ol.geom.LineString([lonlat1, location1]),
                    name: 'Line',
                    //style: styles
                });

                line.setStyle(styles);
                var layer1 = new ol.layer.Vector({
                   source: new ol.source.Vector({
                       features: [
                           line
                       ]
                })
               });
                layer1.setZIndex(0.5);
                return layer1;
}

function LoadDoc()
{

  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function()
  {
    if(this.readyState==4 && this.status == 200)
      {
        fun(JSON.parse(this.response));

      }
  };
  var a = document.getElementById('val').value;
  var b = document.getElementById('date').value;
  if(b === "")return;

  xhttp.open("POST", "_find_bottleneck", true);
  var k = b.split("-")[0] + b.split("-")[1] + b.split("-")[2];
  //console.log(k);
  xhttp.send(k +  " " + a);

}


function fun(dat)
{
  //console.log(map);
  map.setLayerGroup(new ol.layer.Group());
  olms.apply(map, styleJson);

for (i in dat)
  {
    if(dat[i][1] == 0 || dat[i][2] == 0){
      dat[i][1] = dat[i][4];
      dat[i][2] = dat[i][5];
    }
    if(dat[i][4] == 0 || dat[i][5] == 0){
      dat[i][4] = dat[i][1];
      dat[i][5] = dat[i][2];
    }
    map.addLayer(plotLine(new Node(dat[i][0],parseFloat(dat[i][1]), parseFloat(dat[i][2])), new Node(dat[i][3],parseFloat(dat[i][4]), parseFloat(dat[i][5])), parseFloat(dat[i][7])/parseFloat(dat[i][6])*0.9-0.1));
    map.addLayer(plotPoint(new Node(dat[i][0],parseFloat(dat[i][1]), parseFloat(dat[i][2]))));
    map.addLayer(plotPoint(new Node(dat[i][3],parseFloat(dat[i][4]), parseFloat(dat[i][5]))));
  }
}
