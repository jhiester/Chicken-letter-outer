String getPage() {
  String page = "<!DOCTYPE html>";
    page += "<html lang='en'>";
    page += "<head>";
    page += "    <meta charset='UTF-8'>";
    page += "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    page += "    <meta http-equiv='X-UA-Compatible' content='ie=edge'>";
    page += "    <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css'";
    page += "        integrity='sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T' crossorigin='anonymous'>";
    page += "    <link rel='stylesheet' href='./style.css'>";
    page += "    <title>chicken letter outer</title>";
    page += "</head>";
    page += "<body>";
    page += "    <div class='container'>";
    page += "        <div class='row'>";
    page += "            <div class='col'>";
    page += "                <form action='/lockup' method='POST'>";
    page += "                    <button type='button' class='btn btn-outline-dark mt-3'>let the bitches out</button>";
    page += "                </form>";
    page += "                <form action='/freerange' method='POST'>";
    page += "                    <button type='button' class='btn btn-outline-dark mt-3'>lock the bitches up</button>";
    page += "                </form>";
    page += "            </div>";
    page += "        </div>";
    page += "    </div>";
    page += "    <script src='https://code.jquery.com/jquery-3.3.1.slim.min.js'";
    page += "        integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo'";
    page += "        crossorigin='anonymous'></script>";
    page += "    <script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js'";
    page += "        integrity='sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1'";
    page += "        crossorigin='anonymous'></script>";
    page += "    <script src='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js'";
    page += "        integrity='sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM'";
    page += "        crossorigin='anonymous'></script>";
    page += "</body>";
    page += "</html>";

  return page;
}
