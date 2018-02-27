let str = ReasonReact.stringToElement;

type stateType = {rawJson: string};

type action =
  | NoUpdate
  | AddRecord(string);

let component = ReasonReact.reducerComponent("LinkShow");

let fetchLink = id =>
  Js.Promise.(
    Fetch.fetch("https://www.speedrun.com/api/v1/" ++ id)
    |> then_(Fetch.Response.text)
    |> then_(jsonText => resolve(jsonText))
  );

let make = (~link: string, _children) => {
  ...component,
  initialState: () => {rawJson: ""},
  reducer: (action, _state) =>
    switch action {
    | NoUpdate => ReasonReact.NoUpdate
    | AddRecord(json) => ReasonReact.Update({rawJson: json})
    },
  didMount: self => {
    Js.log(link);
    let handleLinkLoaded =
      self.reduce(data => {
        Js.log(data);
        AddRecord(data);
      });
    fetchLink(link)
    |> Js.Promise.then_(data => {
         handleLinkLoaded(data);
         Js.Promise.resolve(data);
       })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  render: _self =>
    <div className="LinkShow"> <h3> (str("LinkShow page")) </h3> </div>
};