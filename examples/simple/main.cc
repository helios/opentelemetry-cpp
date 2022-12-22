// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/exporters/otlp/otlp_http_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_http_exporter_options.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"

#ifdef BAZEL_BUILD
#  include "examples/common/foo_library/foo_library.h"
#else
#  include "foo_library/foo_library.h"
#endif

namespace trace_api      = opentelemetry::trace;
namespace trace_sdk      = opentelemetry::sdk::trace;

namespace
{
    opentelemetry::exporter::otlp::OtlpHttpExporterOptions opts;

void initTracer()
{
  opts.url = "https://collector.heliosphere.io/traces";
  // opts.url = "http://localhost:8080/post";
  // opts.content_type = opentelemetry::exporter::otlp::HttpRequestContentType::kBinary;
  opts.console_debug = true;
  opts.http_headers.insert(
      std::make_pair<const std::string, std::string>("Authorization", "6472e5bea9ef0a90c80f"));
  auto exporter = opentelemetry::exporter::otlp::OtlpHttpExporterFactory::Create(opts);
  auto processor = opentelemetry::sdk::trace::SimpleSpanProcessorFactory::Create(std::move(exporter));
  std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
      trace_sdk::TracerProviderFactory::Create(std::move(processor));

  // Set the global trace provider
  trace_api::Provider::SetTracerProvider(provider);
}
}  // namespace

int main()
{
  // Removing this line will leave the default noop TracerProvider in place.
  initTracer();

  foo_library();
}
