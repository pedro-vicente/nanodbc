#include "example_unicode_utils.h"
#include <nanodbc/nanodbc.h>

#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;
using namespace nanodbc;

void show(nanodbc::result& results);
void run(nanodbc::string const& connection_string, nanodbc::string const& sql);

/////////////////////////////////////////////////////////////////////////////////////////////////////
//main
/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    return EXIT_FAILURE;
  }
  try
  {
    auto const connection_string(convert(argv[1]));
    auto const sql(convert(argv[2]));
    run(connection_string, sql);
    return EXIT_SUCCESS;
  }
  catch (const exception& e)
  {
    cerr << e.what() << endl;
  }
  return EXIT_FAILURE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//run
/////////////////////////////////////////////////////////////////////////////////////////////////////

void run(nanodbc::string const& connection_string, nanodbc::string const& sql)
{
  nanodbc::connection conn(connection_string);
  cout << "Connected with driver " << convert(conn.driver_name()) << endl;
  nanodbc::result results = execute(conn, sql);
  show(results);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//show
/////////////////////////////////////////////////////////////////////////////////////////////////////

void show(nanodbc::result& results)
{
  const short columns = results.columns();
  long rows_displayed = 0;

  cout << "\nDisplaying " << results.affected_rows() << " rows "
       << "(" << results.rowset_size() << " fetched at a time):" << endl;

  cout << "row\t";
  for (short i = 0; i < columns; ++i)
    cout << convert(results.column_name(i)) << "\t";
  cout << endl;

  nanodbc::string const null_value = NANODBC_TEXT("null");
  while (results.next())
  {
    cout << rows_displayed++ << "\t";
    for (short col = 0; col < columns; ++col)
    {
      auto const value = results.get<nanodbc::string>(col, null_value);
      cout << "(" << convert(value) << ")\t";
    }
    cout << endl;
  }
}


