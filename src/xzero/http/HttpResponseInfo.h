// This file is part of the "x0" project, http://xzero.io/
//   (c) 2009-2014 Christian Parpart <trapni@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <xzero/http/Api.h>
#include <xzero/sysconfig.h>
#include <xzero/http/HttpInfo.h>
#include <xzero/http/HttpStatus.h>
#include <string>

namespace xzero {
namespace http {

/**
 * HTTP Response Message Info.
 */
class XZERO_HTTP_API HttpResponseInfo : public HttpInfo {
 public:
  HttpResponseInfo();
  HttpResponseInfo(HttpResponseInfo&& other);
  HttpResponseInfo& operator=(HttpResponseInfo&& other);

  HttpResponseInfo(HttpVersion version, HttpStatus status,
                   const std::string& reason, bool isHeadResponse,
                   size_t contentLength,
                   const HeaderFieldList& headers,
                   const HeaderFieldList& trailers);

  /** Retrieves the HTTP response status code. */
  HttpStatus status() const XZERO_NOEXCEPT { return status_; }

  const std::string& reason() const XZERO_NOEXCEPT { return reason_; }

  /** Retrieves whether this is an HTTP response to a HEAD request. */
  bool isHeadResponse() const XZERO_NOEXCEPT { return isHeadResponse_; }

 private:
  HttpStatus status_;
  std::string reason_;
  bool isHeadResponse_;
};

inline HttpResponseInfo::HttpResponseInfo()
    : HttpResponseInfo(HttpVersion::UNKNOWN, HttpStatus::Undefined, "", false,
                       0, {}, {}) {
}

inline HttpResponseInfo::HttpResponseInfo(HttpResponseInfo&& other)
  : HttpResponseInfo(other.version_, other.status_, "", other.isHeadResponse_,
                     other.contentLength_, {}, {}) {

  reason_.swap(other.reason_);
  headers_.swap(other.headers_);
  trailers_.swap(other.trailers_);
  other.contentLength_ = 0;
}

inline HttpResponseInfo& HttpResponseInfo::operator=(HttpResponseInfo&& other) {
  version_ = other.version_;
  contentLength_ = other.contentLength_;
  headers_ = std::move(other.headers_);
  trailers_ = std::move(other.trailers_);

  status_ = other.status_;
  reason_ = std::move(other.reason_);
  isHeadResponse_ = other.isHeadResponse_;

  return *this;
}

inline HttpResponseInfo::HttpResponseInfo(HttpVersion version,
                                          HttpStatus status,
                                          const std::string& reason,
                                          bool isHeadResponse,
                                          size_t contentLength,
                                          const HeaderFieldList& headers,
                                          const HeaderFieldList& trailers)
    : HttpInfo(version, contentLength, headers, trailers),
      status_(status),
      reason_(reason),
      isHeadResponse_(isHeadResponse) {
  //.
}

}  // namespace http
}  // namespace xzero
