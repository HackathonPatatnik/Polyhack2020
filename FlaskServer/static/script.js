


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

/*var nodedata = "AA, 8.05127371849, 47.3913594962" +"\n"+ "AAO, 8.07230995478, 47.3952500261" + "\n"+ "AAT, 8.76562510867, 47.3359591251" + "\n"+ "ABE, 7.27869253926, 47.0445413042";
document.write(nodedata);
var arr = nodedata.split("\n");
document.write(3);
let nodes = [];
//var nodes = new Node[1000]();
var size=0;
for(i in arr)
  {
    var t = i.split(",");
    document.write(i);
    /*var k = t[0];
    var lat = (String)t[1].toInt();
    var lng = t[2].toInt();
    nodes.push(Node(k, lat, lng));
    size++;*/
 // }
//document.write(size);


  var styleJson = 'https://api.maptiler.com/maps/e37057d7-2e5c-4978-838e-ce1022159874/style.json?key=oB33yoiaSZxO5iGNOflP';
  var map = new ol.Map({
          target: 'map',
          view: new ol.View({
            center: ol.proj.fromLonLat([7.6280, 46.7580]),
            zoom: 8
          })
        });
  olms.apply(map, styleJson);









/*var json1 = '{\"0\" : {\"coordx\" : \"8.05127371849\",\"coordy\" : \"57.3913594962\",\"cap:\" :\"1\" }, \"1\" : {\"coordx\" :\"28.05127371849\",\"coordy\" : \"37.3913594962\",\"cap:\" : \"0.5\"\"},\"2\" : {\"coordx\" :\"18.05127371849\",\"coordy\" :\"47.3913594962\",\"cap:\" : \"0.3\"}}';
var t = JSON.parse(json1);

for(i in t)
  {
    plotPoint(new Node(i.coordx, i.coordy, i.col));
  }

var t1 = JSON.parse(json2);

for(i in t1)
  {
    plotPoint(new Node(i.coordx, i.coordy, i.col));
  }*/









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
                //layer1.setOpacity(0.5);
                return layer1;
}
//map.addLayer(plotLine(nodes, nodes, 0));












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
  console.log(k);

  xhttp.send(k +  " " + a);

}


function fun(dat)
{
  console.log(map);
  map.setLayerGroup(new ol.layer.Group());
  olms.apply(map, styleJson);


for (i in dat)
  {
    //To Do color
    //map.addLayer(plotLine(new Node(parseFloat("7.06245667714"), parseFloat("48.1544144565")), new Node(parseFloat("7.24291053739"), parseFloat("47.1328978528")), 0));
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





// Create a GeoAdmin Map
/*var map = new ga.Map({
  // Define the div where the map is placed
  target: 'map',
  view: new ol.View({
    resolution: 250,
     center: [2660000, 1190000]
  }),

});

//background
var lyr = ga.layer.create('ch.swisstopo.swissboundaries3d-land-flaeche.fill');
lyr.setOpacity(0.2);
//lyr.setHue(0.5);
map.addLayer(lyr);*/

//var lyr = ga.layer.create();


/*

 var layer = new ol.layer.Tile({
          source: new ol.source.OSM()
        });

        // create an interaction to add to the map that isn't there by default
        var interaction = new ol.interaction.DragRotateAndZoom();

        // create a control to add to the map that isn't there by default
        var control = new ol.control.FullScreen();

        // center on london, transforming to map projection
        var center = ol.proj.transform([-1.812, 52.443], 'EPSG:4326', 'EPSG:3857');

        // an overlay to position at the center
        var overlay = new ol.Overlay({
          position: center,
          element: document.getElementById('overlay')
        });

        // view, starting at the center
        var view = new ol.View({
          center: center,
          zoom: 6
        });

        // finally, the map with our custom interactions, controls and overlays
        var map = new ol.Map({
          target: 'map',
          layers: [layer],
          interactions: [interaction],
          controls: [control],
          overlays: [overlay],
          view: view
        });
        */
