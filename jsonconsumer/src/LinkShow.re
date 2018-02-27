let str = ReasonReact.stringToElement;

type stateType = {
  rawJson: string
};

type action = 
  | NoUpdate
  | AddRecord(string);

let component = ReasonReact.reducerComponent("LinkShow");

let make = (~link: string, _children) => {
  ...component,
  initialState: () => {
    rawJson: ""
  },
  reducer: (action, state) => 
    switch action {
    | NoUpdate => ReasonReact.NoUpdate
    | AddRecord(json) => ReasonReact.Update({rawJson: json})
  },
  render: _self => {
    Js.log(link);
    <div className="LinkShow">
      <h3>(str("LinkShow"))</h3>
    </div>
  }
};