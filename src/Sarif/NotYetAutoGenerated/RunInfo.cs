// Copyright (c) Microsoft.  All Rights Reserved.
// Licensed under the Apache License, Version 2.0.  See License.txt in the project root for license information.

using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;

namespace Microsoft.CodeAnalysis.Sarif
{
    /// <summary>
    /// A runInfo object describes the invocation of the static analysis tool that produced the results specified in the containing runLog object. NOTE: The information in the runInfo object makes it possible to precisely repeat a run of a static analysis tool, and to verify that the results reported in the log file were generated by an appropriate invocation of the tool.
    /// </summary>
    [DataContract, GeneratedCode("Microsoft.Json.Schema.ToDotNet", "0.6.0.0")]
    public partial class RunInfo : ISyntax, IEquatable<RunInfo>
    {
        /// <summary>
        /// Gets a value indicating the type of object implementing <see cref="ISyntax" />.
        /// </summary>
        public SarifKind SyntaxKind
        {
            get
            {
                return SarifKind.RunInfo;
            }
        }

        /// <summary>
        /// A string that describes any parameterization for the tool invocation. For command line tools this string may consist of the completely specified command line used to invoke the tool.
        /// </summary>
        [DataMember(Name = "invocationInfo", IsRequired = false, EmitDefaultValue = false)]
        public string InvocationInfo { get; set; }

        /// <summary>
        /// A dictionary each of whose keys is a URI and each of whose values is an array of fileReference objects representing the location of a single file target scanned during the run.
        /// </summary>
        /// <remarks>
        /// Json.Schema.ToDotNet doesn't yet handle dictionaries whose values are anything other than strings.
        /// </remarks>
        [DataMember(Name = "fileInfo", IsRequired = false, EmitDefaultValue = false)]
        public Dictionary<string, IList<FileReference>> FileInfo { get; set; }

        /// <summary>
        /// An optional string specifying the date and time at which the run started.
        /// </summary>
        [DataMember(Name = "runStartTime", IsRequired = false, EmitDefaultValue = false)]
        public DateTime RunStartTime { get; set; }

        /// <summary>
        /// An optional string specifying the date and time at which the run ended.
        /// </summary>
        [DataMember(Name = "runEndTime", IsRequired = false, EmitDefaultValue = false)]
        public DateTime RunEndTime { get; set; }

        /// <summary>
        /// An optional string identifier that allows the run to be associated with a larger automation process.
        /// </summary>
        [DataMember(Name = "correlationId", IsRequired = false, EmitDefaultValue = false)]
        public string CorrelationId { get; set; }

        /// <summary>
        /// An optional string identifier that specifies the hardware architecture for which the run was targeted.
        /// </summary>
        [DataMember(Name = "architecture", IsRequired = false, EmitDefaultValue = false)]
        public string Architecture { get; set; }

        /// <summary>
        /// Key/value pairs that provide additional details about the run.
        /// </summary>
        [DataMember(Name = "properties", IsRequired = false, EmitDefaultValue = false)]
        public Dictionary<string, string> Properties { get; set; }

        /// <summary>
        /// A unique set of strings that provide additional information for the run.
        /// </summary>
        [DataMember(Name = "tag", IsRequired = false, EmitDefaultValue = false)]
        public IList<string> Tag { get; set; }

        public override bool Equals(object other)
        {
            return Equals(other as RunInfo);
        }

        public override int GetHashCode()
        {
            int result = 17;
            unchecked
            {
                if (InvocationInfo != null)
                {
                    result = (result * 31) + InvocationInfo.GetHashCode();
                }

                if (FileInfo != null)
                {
                    // Use xor for dictionaries to be order-independent.
                    int xor_99 = 0;
                    foreach (var value_99 in FileInfo)
                    {
                        xor_99 ^= (value_99.Key ?? string.Empty).GetHashCode();
                        xor_99 ^= (value_99.Value ?? new List<FileReference>()).GetHashCode();
                    }

                    result = (result * 31) + xor_99;
                }

                result = (result * 31) + RunStartTime.GetHashCode();
                result = (result * 31) + RunEndTime.GetHashCode();
                if (CorrelationId != null)
                {
                    result = (result * 31) + CorrelationId.GetHashCode();
                }

                if (Architecture != null)
                {
                    result = (result * 31) + Architecture.GetHashCode();
                }

                if (Properties != null)
                {
                    // Use xor for dictionaries to be order-independent.
                    int xor_0 = 0;
                    foreach (var value_0 in Properties)
                    {
                        xor_0 ^= (value_0.Key ?? string.Empty).GetHashCode();
                        xor_0 ^= (value_0.Value ?? string.Empty).GetHashCode();
                    }

                    result = (result * 31) + xor_0;
                }

                if (Tag != null)
                {
                    foreach (var value_1 in Tag)
                    {
                        result = result * 31;
                        if (value_1 != null)
                        {
                            result = (result * 31) + value_1.GetHashCode();
                        }
                    }
                }
            }

            return result;
        }

        public bool Equals(RunInfo other)
        {
            if (other == null)
            {
                return false;
            }

            if (InvocationInfo != other.InvocationInfo)
            {
                return false;
            }

            if (!Object.ReferenceEquals(FileInfo, other.FileInfo))
            {
                if (FileInfo == null || other.FileInfo == null || FileInfo.Count != other.FileInfo.Count)
                {
                    return false;
                }

                foreach (var value_99 in FileInfo)
                {
                    IList<FileReference> value_100;
                    if (!other.FileInfo.TryGetValue(value_99.Key, out value_100))
                    {
                        return false;
                    }

                    var firstNotSecond = value_99.Value.Except(value_100);
                    if (firstNotSecond.Any())
                    {
                        return false;
                    }

                    var secondNotFirst = value_100.Except(value_99.Value);
                    if (secondNotFirst.Any())
                    {
                        return false;
                    }
                }
            }

            if (RunStartTime != other.RunStartTime)
            {
                return false;
            }

            if (RunEndTime != other.RunEndTime)
            {
                return false;
            }

            if (CorrelationId != other.CorrelationId)
            {
                return false;
            }

            if (Architecture != other.Architecture)
            {
                return false;
            }

            if (!Object.ReferenceEquals(Properties, other.Properties))
            {
                if (Properties == null || other.Properties == null || Properties.Count != other.Properties.Count)
                {
                    return false;
                }

                foreach (var value_0 in Properties)
                {
                    string value_1;
                    if (!other.Properties.TryGetValue(value_0.Key, out value_1))
                    {
                        return false;
                    }

                    if (value_0.Value != value_1)
                    {
                        return false;
                    }
                }
            }

            if (!Object.ReferenceEquals(Tag, other.Tag))
            {
                if (Tag == null || other.Tag == null)
                {
                    return false;
                }

                if (Tag.Count != other.Tag.Count)
                {
                    return false;
                }

                for (int value_2 = 0; value_2 < Tag.Count; ++value_2)
                {
                    if (Tag[value_2] != other.Tag[value_2])
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="RunInfo" /> class.
        /// </summary>
        public RunInfo()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="RunInfo" /> class from the supplied values.
        /// </summary>
        /// <param name="invocationInfo">
        /// An initialization value for the <see cref="P: InvocationInfo" /> property.
        /// </param>
        /// <param name="fileInfo">
        /// An initialization value for the <see cref="P: FileInfo" /> property.
        /// </param>
        /// <param name="runStartTime">
        /// An initialization value for the <see cref="P: RunStartTime" /> property.
        /// </param>
        /// <param name="runEndTime">
        /// An initialization value for the <see cref="P: RunEndTime" /> property.
        /// </param>
        /// <param name="correlationId">
        /// An initialization value for the <see cref="P: CorrelationId" /> property.
        /// </param>
        /// <param name="architecture">
        /// An initialization value for the <see cref="P: Architecture" /> property.
        /// </param>
        /// <param name="properties">
        /// An initialization value for the <see cref="P: Properties" /> property.
        /// </param>
        /// <param name="tag">
        /// An initialization value for the <see cref="P: Tag" /> property.
        /// </param>
        public RunInfo(string invocationInfo, Dictionary<string, IList<FileReference>> fileInfo, DateTime runStartTime, DateTime runEndTime, string correlationId, string architecture, Dictionary<string, string> properties, IEnumerable<string> tag)
        {
            Init(invocationInfo, fileInfo, runStartTime, runEndTime, correlationId, architecture, properties, tag);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="RunInfo" /> class from the specified instance.
        /// </summary>
        /// <param name="other">
        /// The instance from which the new instance is to be initialized.
        /// </param>
        /// <exception cref="ArgumentNullException">
        /// Thrown if <paramref name="other" /> is null.
        /// </exception>
        public RunInfo(RunInfo other)
        {
            if (other == null)
            {
                throw new ArgumentNullException(nameof(other));
            }

            Init(other.InvocationInfo, other.FileInfo, other.RunStartTime, other.RunEndTime, other.CorrelationId, other.Architecture, other.Properties, other.Tag);
        }

        ISyntax ISyntax.DeepClone()
        {
            return DeepCloneCore();
        }

        /// <summary>
        /// Creates a deep copy of this instance.
        /// </summary>
        public RunInfo DeepClone()
        {
            return (RunInfo)DeepCloneCore();
        }

        private ISyntax DeepCloneCore()
        {
            return new RunInfo(this);
        }

        private void Init(string invocationInfo, Dictionary<string, IList<FileReference>> fileInfo, DateTime runStartTime, DateTime runEndTime, string correlationId, string architecture, Dictionary<string, string> properties, IEnumerable<string> tag)
        {
            InvocationInfo = invocationInfo;
            if (fileInfo != null)
            {
                FileInfo = new Dictionary<string, IList<FileReference>>(fileInfo);
            }

            RunStartTime = runStartTime;
            RunEndTime = runEndTime;
            CorrelationId = correlationId;
            Architecture = architecture;
            if (properties != null)
            {
                Properties = new Dictionary<string, string>(properties);
            }

            if (tag != null)
            {
                var destination_0 = new List<string>();
                foreach (var value_0 in tag)
                {
                    destination_0.Add(value_0);
                }

                Tag = destination_0;
            }
        }
    }
}