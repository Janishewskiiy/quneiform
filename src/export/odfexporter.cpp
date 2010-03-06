/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <ctime>
#include "odfexporter.h"
#include "ced/cedchar.h"
#include "common/tostring.h"
#include "compat/filefunc.h"
#include "config.h" // for CF_VERSION
namespace CIF
{

std::string datetime(const std::string& format = "%Y-%m-%dT%H:%M:%S") {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[100];
    const size_t buffer_length = sizeof(buffer) - 2;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, buffer_length, format.c_str(), timeinfo);
    buffer[buffer_length - 1] = '\0';
    return buffer;
}

OdfExporter::OdfExporter(CEDPage * page, const FormatOptions& opts) :
    TextExporter(page, opts), zip_(NULL) {
}

OdfExporter::~OdfExporter() {
    odfClose();
}

void OdfExporter::addOdfContent() {
    std::ostringstream buf;
    buf << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<office:document-content xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
        " xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\""
        " xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\""
        " xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
        " xmlns:xlink=\"http://www.w3.org/1999/xlink\""
        " xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
        " xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\""
        " xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\""
        " office:version=\"1.2\">"
        "<office:body>";

    doExport(buf);
    buf << "</office:body></office:document-content>";
    odfWrite("content.xml", buf.str());
}

void OdfExporter::addOdfManifest() {
    zip_add_dir(zip_, "META-INF");

    std::ostringstream buf;
    buf
            << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                "<manifest:manifest xmlns:manifest=\"urn:oasis:names:tc:opendocument:xmlns:manifest:1.0\">"
                "<manifest:file-entry manifest:media-type=\"application/vnd.oasis.opendocument.text\" manifest:version=\"1.0\" manifest:full-path=\"/\"/>"
                "<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"content.xml\"/>"
                "<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"styles.xml\"/>"
                "<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"meta.xml\"/>"
                "<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"settings.xml\"/>"
                "</manifest:manifest>";
    odfWrite("META-INF/manifest.xml", buf.str());
}

void OdfExporter::addOdfMeta() {
    std::ostringstream buf;
    std::string time = datetime();
    buf << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<office:document-meta xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
        " xmlns:xlink=\"http://www.w3.org/1999/xlink\""
        " xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
        " xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\""
        " office:version=\"1.2\">"
        "<office:meta><meta:creation-date>" << time << "</meta:creation-date>"
        "<meta:document-statistic meta:table-count=\"0\" meta:image-count=\"0\""
        " meta:object-count=\"0\" meta:page-count=\"1\" meta:paragraph-count=\"0\""
        " meta:word-count=\"0\" meta:character-count=\"0\"/>"
        " <dc:date>" << time << "</dc:date>";
    buf << "<meta:generator>Cuneiform-" << CF_VERSION << "</meta:generator>";
    buf << "</office:meta></office:document-meta>";

    odfWrite("meta.xml", buf.str());
}

void OdfExporter::addOdfMime() {
    odfWrite("mimetype", "application/vnd.oasis.opendocument.text");
}

void OdfExporter::addOdfSettings() {
    std::ostringstream buf;
    buf << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<office:document-settings xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\"/>";
    odfWrite("settings.xml", buf.str());
}

void OdfExporter::addOdfStyles() {
    std::ostringstream buf;
    buf << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<office:document-style xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
        " office:version=\"1.2\""
        " xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\""
        " xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\"/>";
    odfWrite("styles.xml", buf.str());
}

void OdfExporter::exportTo(const std::string& fname) {
    unlink(fname.c_str());
    odfOpen(fname);
    addOdfMime();
    addOdfMeta();
    addOdfSettings();
    addOdfStyles();
    addOdfContent();
    addOdfManifest();
    odfClose();
}

void OdfExporter::exportTo(std::ostream& os) {
    std::string tmpfile = "tmp.odt.$$$";
    exportTo(tmpfile);
    std::ifstream f(tmpfile.c_str());
    os << f.rdbuf();
    unlink(tmpfile.c_str());
}

void OdfExporter::odfClose() {
    zip_close(zip_);
    zip_ = NULL;
    buffers_.clear();
}

void OdfExporter::odfOpen(const std::string& fname) {
    int zip_error = 0;
    zip_ = zip_open(fname.c_str(), ZIP_CREATE, &zip_error);
    if (!zip_) {
        switch (zip_error) {
        case ZIP_ER_EXISTS:
            throw Exception("[OdfExporter::odfOpen] file already exists: " + fname);
        default:
            throw Exception("[OdfExporter::odfOpen] can't open file: " + fname);
        }
    }
}

void OdfExporter::odfWrite(const std::string& fname, const std::string& data) {
    assert(zip_);
    BufList::iterator it = buffers_.insert(buffers_.end(), data);
    zip_source * src = zip_source_buffer(zip_, it->c_str(), it->size(), 0);

    if (!src)
        throw Exception("[OdfExporter::odfWrite] error: " + std::string(zip_strerror(zip_)));

    if (zip_add(zip_, fname.c_str(), src) < 0) {
        zip_source_free(src);
        throw Exception("[OdfExporter::addOdfMime] error: " + std::string(zip_strerror(zip_)));
    }
}

void OdfExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    assert(chr && chr->alternatives);
    //writeFontStyle(os, chr->fontAttribs);
    os << escapeXmlSpecialChar(chr->alternatives->alternative);
}

void OdfExporter::writePageBegin(std::ostream& os) {
    os << "<office:text>";
}

void OdfExporter::writePageEnd(std::ostream& os) {
    os << "</office:text>";
}

void OdfExporter::writeParagraphBegin(std::ostream& os, CEDParagraph * par) {
    os << "<text:p>";
}

void OdfExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * par) {
    os << "</text:p>";
}

}
