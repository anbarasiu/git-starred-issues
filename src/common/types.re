type repository = {
    id: int,
    name: string,
    full_name: string,
    html_url: string,
    open_issues_count: int,
    issues_url: string,
    labels_url: string,
    url: string
  };
  
  type repositories = array(repository);
  
  type issueLabel = {
    id: int,
    name: string,
    color: string
  };
  
  type issueLabels = array(issueLabel);
  
  type issue = {
    id: int,
    title: string,
    body: string,
    html_url: string,
    state: string,
    labels: issueLabels
  };
  
  type issues = array(issue);