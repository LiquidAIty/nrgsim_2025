
module.exports = {
  drawRoutes: function(app, excel) {

    var _ = require('lodash');
  
    var zone = {
      zone:
        [
          { BuildingSurface:
            { name: "Floor", type: "Floor",
              vertices: [
                { x: 3.4909079445, y: 0.0164379644, z: 0 },
                { x: 0.0099079445, y: 0.0164379644, z: 0 },
                { x: 0.0099079445, y: 3.4974379644, z: 0 },
                { x: 3.4909079445, y: 3.4974379644, z: 0}
              ]
            },
          },
          { BuildingSurface:
            { name: "Ceiling", type: "Ceiling",
              vertices: [
                { x: 0.0099079445, y: 0.0164379644, z: 3.501 },
                { x: 3.4909079445, y: 0.0164379644, z: 3.501 },
                { x: 3.4909079445, y: 3.4974379644, z: 3.501 },
                { x: 0.0099079445, y: 3.4974379644, z: 3.501 }
              ]
            }
          },
          { BuildingSurface:
            { name: "EastWall", type: "Wall",
              vertices: [
                { x: 3.4909079445, y: 0.0164379644, z: 0 },
                { x: 3.4909079445, y: 3.4974379644, z: 0 },
                { x: 3.4909079445, y: 3.4974379644, z: 3.501 },
                { x: 3.4909079445, y: 0.0164379644, z: 3.501 }
              ]
            },
          },
          { BuildingSurface:
            { name: "NorthWall", type: "Wall",
              vertices: [
                { x: 3.4909079445, y: 3.4974379644, z: 0 },
                { x: 0.0099079445, y: 3.4974379644, z: 0 },
                { x: 0.0099079445, y: 3.4974379644, z: 3.501 },
                { x: 3.4909079445, y: 3.4974379644, z: 3.501 }
              ]
            },
          },
          { BuildingSurface:
            { name: "WestWall", type: "Wall",
               vertices: [
                { x: 0.0099079445, y: 3.4974379644, z: 0 },
                { x: 0.0099079445, y: 0.0164379644, z: 0 },
                { x: 0.0099079445, y: 0.0164379644, z: 3.501 },
                { x: 0.0099079445, y: 3.4974379644, z: 3.501 }
              ]
            }
          },
          { BuildingSurface:
            { name: "SouthWall", type: "Wall",
              vertices: [
                { x: 0.0099079445, y: 0.0164379644, z: 0 },
                { x: 3.4909079445, y: 0.0164379644, z: 0 },
                { x: 3.4909079445, y: 0.0164379644, z: 3.501 },
                { x: 0.0099079445, y: 0.0164379644, z: 3.501 }
              ]
            },
          },
          { FenestrationSurface:
            { name: "EastWindow", type: "Window",
               vertices: [
                { x: 3.4909079445, y: 0.07143796, z: 1.04 },
                { x: 3.4909079445, y: 3.44243796, z: 1.04 },
                { x: 3.4909079445, y: 3.44243796, z: 2.3715738626 },
                { x: 3.4909079445, y: 0.07143796, z: 2.3715738626 }
              ]
            }
          }
        ]
    };

    app.post('/user/session', function(req, res) {
      var email = req.body.email;
      var pwd = req.body.password;
      if (email === pwd) {
        var user = { _id: 1, __v: 0, name: req.body.name, email: email, password: pwd };
        res.status(200).json(user);
      } else {
        res.status(400).json({"msg": "login failed"});
      }
    });

    app.delete('/user/session', function(req, res) {
      res.status(200).json({ "message" : "logged out" });
    });

    app.post('/user', function(req, res) {
      if (req.body.email === 'taken@taken.com') {
        res.status(400).json({ message: "That email address is already taken." });
      } else {
        res.status(200).json({ "message" : "user registered" });
      }
    });

    app.put('/user', function(req, res) {
      res.status(200).json({ "message" : "user updated" });
    });

    app.get('/simulation/load/:id', function(req, res) {
      res.json({ "status": 0, "data": zone});
    });
  
    app.post('/simulation/run', function(req, res) {
      console.log(JSON.stringify(req.body, null, '  '));
      res.json({ "status": 0, "data": "this would be some simulation results"});
    });

    app.get('/simulation/:jobId', function(req, res) {
      var jobId = req.params.jobId;
      var rnd = Math.floor((Math.random() * 4) + 1);
      var finished = (rnd === 1);
      res.send("This would be the results streamed back from the server");
    });
  }
};