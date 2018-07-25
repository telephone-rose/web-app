[@bs.deriving abstract]
type _coords = {
  latitude: float,
  longitude: float,
};

[@bs.deriving abstract]
type _position = {coords: _coords};

[@bs.scope ("navigator", "geolocation")] [@bs.val]
external _getCurrentPosition : (_position => unit) => unit =
  "getCurrentPosition";

type position = {
  latitude: float,
  longitude: float,
};

let getCurrentPosition = callback =>
  _getCurrentPosition(_position =>
    {
      latitude: _position |> coordsGet |> latitudeGet,
      longitude: _position |> coordsGet |> longitudeGet,
    }
    |> callback
  );
