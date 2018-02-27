let str = ReasonReact.stringToElement;

type routes =
  | LinksIndex
  | LinkShow(string);

type action =
  | NoUpdate
  | ChangeRoute(routes)
  | ShowLinksIndex
  | ShowLinkShow(string);

type stateType = {currentRoute: routes};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {currentRoute: LinksIndex},
  reducer: (action, _state) =>
    switch action {
    | NoUpdate => ReasonReact.NoUpdate
    | ChangeRoute(route) => ReasonReact.Update({currentRoute: route})
    | ShowLinksIndex => ReasonReact.Update({currentRoute: LinksIndex})
    | ShowLinkShow(linkName) =>
      ReasonReact.Update({currentRoute: LinkShow(linkName)})
    },
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url => {
          Js.log(url.path);
          switch url.path {
          | ["links", linkName] => self.send(ShowLinkShow(linkName))
          | _ => self.send(ShowLinksIndex)
          };
        }),
      ReasonReact.Router.unwatchUrl
    )
  ],
  didMount: self => {
    let url = ReasonReact.Router.dangerouslyGetInitialUrl();
    let handleGetCurrentRoute = self.reduce(route => ChangeRoute(route));
    (
      switch url.path {
      | ["links", linkName] => LinkShow(linkName)
      | _ => LinksIndex
      }
    )
    |> handleGetCurrentRoute;
    ReasonReact.NoUpdate;
  },
  render: self => {
    let currentView =
      switch self.state.currentRoute {
      | LinkShow(linkName) => <LinkShow link=linkName />
      | LinksIndex => <LinksIndex />
      };
    <div className="App">
      <div className="App-header">
        <h2 className="App-intro"> (str("Title")) </h2>
      </div>
      currentView
    </div>;
  }
};